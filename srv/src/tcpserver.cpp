/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP server for C++.
 */

#include <tcpserver.hpp>
#include <tcpserver.h>

#include <cstring>

namespace TCP
{
  Server::Server(const unsigned short port, const void* env)
    : Common(port, env)
    , server(tcpopen(port, messages, errors))
    , connections{NULL}
  {
    server->env = this;
  }

  Server::~Server()
  {
    tcpclose(server);
    for (int i = 0; i < TCP_MAX_CONN; i++)
    {
      if (connections[i]) delete connections[i];
    }
  }

  void Server::start()
  {
    tcpaccept(server, accept);
  }

  void Server::stop()
  {
    tcpcancel(server);
  }

  const Connection& Server::getConnection(int ID)
  {
    return *connections[ID];
  }

  void Server::accept(TCPServer* server, int clientID)
  {
    Server* this_server = (Server*)server->env;

    if (this_server->connections[clientID])  delete this_server->connections[clientID];
    this_server->connections[clientID] = new Connection(server->connections[clientID], this_server->env);

    this_server->onConnect.trigger(*this_server->connections[clientID]);
  }
}
