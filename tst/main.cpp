/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Simple test for C++.
 */

#include <udp.hpp>

#include <iostream>
#include <cstring>

struct Env {
  std::string client;
};

void receiveMessage(UDP::Message &message, Env* env) {
  std::string m((char*)message.bytes, message.size);
  std::cout << "From " << message.address.address_str() << 
                   "(" << message.address.port() << "): " << m << std::endl;
  env->client = std::to_string(message.address);
}

int main(int argc, char* argv[]) {
  UDP::Common* common;
  Env env    = { "" };
  int client = 0;

  if (argc > 1) {
    if(!std::strcmp(argv[1], "-s")) {
      common = new UDP::Server(8008, env);
    } else if (!std::strcmp(argv[1], "-c")) {
      common = new UDP::Client(8008, 8009, env);
      client = 1;
    }
  }

  common->onReceive += (UDP::Callback)receiveMessage;

  common->start();
  std::cout << common->message() << std::endl;
  std::cout << "To quit send 'q' string.\n";
  while (true) {
    std::string line;
    std::getline(std::cin, line);
    if (line == "q") break;
    if (env.client == "" && !client) {
      std::cerr << "Cannot send a message because no client has sent anything yet.\n";
    } else {
      if (client) ((UDP::Client*)common)->send(line);
      else        ((UDP::Server*)common)->send(env.client.c_str(), line);
    }
  }
  common->stop();
  delete common;
  return 0;
}