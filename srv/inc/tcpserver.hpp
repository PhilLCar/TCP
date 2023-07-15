/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP server for C++.
 */

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <tcpcommon.hpp>
#include <tcpconnection.hpp>

#include <string>

namespace TCP {
  typedef void (*ConnectionCallback)(Connection&);

  class ConnectionEvent {
    friend class Server;

  public:
    inline void operator +=(ConnectionCallback callback) {
      callbacks.push_back(callback);
    }
    inline void operator -=(ConnectionCallback callback) {
      callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

  private:
    inline void trigger(Connection& connection) {
      for (auto callback : callbacks) callback(connection);
    }

  private:
    std::vector<ConnectionCallback> callbacks;
  };


  class Server : public Common {
  public:
    inline Server(const unsigned short port) : Server(port, nullptr) {}

    template <typename T>
    inline Server(const unsigned short port, const T& env) : Server(port, (const void*)&env) {}

    ~Server();

  private:
    explicit Server(const unsigned short port, const void* env);

  public:
    void start();
    void stop();

    // might throw an error if ID doesn't exist
    const Connection& getConnection(int ID);

    private:
      static void accept(TCPServer* server, int clientID);
    
    public:
      ConnectionEvent onConnect;

    private:
      TCPServer*  server;
      // TODO: For now connection wrapper, eventually think about onclose event
      Connection* connections[TCP_MAX_CONN];
  };
}

#endif