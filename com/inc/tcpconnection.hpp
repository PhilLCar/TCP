/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 09 Jul 2023
 * Description: TCP connection for C++.
 */

#ifndef TCPCONNECTION_HPP
#define TCPCONNECTION_HPP

#include <tcpcommon.hpp>
#include <IPV4Address.hpp>

#include <vector>
#include <algorithm>

using namespace Networking;

namespace TCP {
    struct Message {
    unsigned char* bytes;
    size_t         size;
    IPV4Address    address;
  };

  typedef void (*ReceptionCallback)(Message&, void*);

  class ReceptionEvent {
    friend class Connection;

  public:
    inline void operator +=(ReceptionCallback callback) {
      callbacks.push_back(callback);
    }
    inline void operator -=(ReceptionCallback callback) {
      callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

  private:
    inline void trigger(Message& message, const void *env) {
      for (auto callback : callbacks) callback(message, (void*)env);
    }

  private:
    std::vector<ReceptionCallback> callbacks;
  };

  class Connection {
  public:
    Connection(TCPConnection* connection, const void* env);
    ~Connection();

  public:
    bool send(const unsigned char* bytes, size_t length);
    bool send(const std::string& message);
    bool send(const char* message);

    template <typename T>
    inline bool send(const T& serialized) {
      return send((unsigned char*)&serialized, sizeof(T));
    }

    template <typename T>
    inline T& getEnv() { return *(T*)env; }

  private:
    static void receive(TCPConnection* connection);

  public:
    ReceptionEvent onReceive;

  private:
    TCPConnection* connection;
    const void*    env;
  };
}

#endif