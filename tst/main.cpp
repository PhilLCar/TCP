/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Simple test for C++.
 */

#include <tcp.hpp>

#include <iostream>
#include <cstring>
#include <csignal>

using namespace TCP;

struct Env {
  Connection*     connection;
  pthread_mutex_t connection_mutex;
  pthread_cond_t  client_connected;
};

// TODO fix all the const calls
void connect(Connection& connection)
{
  Env& env = connection.getEnv<Env>();

  env.connection = &connection;

  std:: cout << "Client connected! Waking up main thread..." << std::endl;
  // Wake up the main thread
  pthread_mutex_lock(&env.connection_mutex);
  pthread_cond_signal(&env.client_connected);
  pthread_mutex_unlock(&env.connection_mutex);
}

void receive(Message message, Env& env)
{
  std::cout << "From (" << std::to_string(message.address) << "): " << (const char*)message.bytes << std::endl;
}

int main(int argc, char* argv[]) {
  Common* common = NULL;
  Env     env;

  env.connection_mutex = PTHREAD_MUTEX_INITIALIZER;
  env.client_connected = PTHREAD_COND_INITIALIZER;

  if (argc > 1) {
    if(!std::strcmp(argv[1], "-s")) {
      Server* server = new Server(8008, env);

      server->onConnect += connect;
      server->start();

      // Wait for one client to be connected
      pthread_mutex_lock(&env.connection_mutex);
      pthread_cond_wait(&env.client_connected, &env.connection_mutex);
      pthread_mutex_unlock(&env.connection_mutex);
      std::cout << "Woke up!" << std::endl;
      common = server;
    } else if (!std::strcmp(argv[1], "-c")) {
      Client* client = new Client(8008);

      common = client;
      env.connection = &client->getConnection();
    }
  }

  if (common)
  {
    std::cout << "Connection established, write your message ('q' to quit):" << std::endl;
    env.connection->onReceive += (ReceptionCallback)receive;

    while (env.connection)
    {
      std::string input;
      std::cin >> input;

      if (input == "q") break;

      env.connection->send(input);
    }

    delete common;
  }
  return 0;
}