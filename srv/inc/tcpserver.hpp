/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP server for C++.
 */

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <common.hpp>

#include <string>

namespace UDP {
  class Server : public Common {
  public:
    inline Server(const unsigned short port) : Server(port, nullptr) {}

    template <typename T>
    inline Server(const unsigned short port, const T& env) : Server(port, (const void*)&env) {}

  private:
    explicit Server(const unsigned short port, const void* env);

  public:
    bool send(const IPV4Address& client, const unsigned char* bytes, size_t length);
    bool send(const IPV4Address& client, const std::string& message);
    bool send(const IPV4Address& client, const char* message);

    template <typename T>
    inline bool send(const IPV4Address& client, const T& serialized) {
      return send(client, (unsigned char*)&serialized, sizeof(T));
    }

    inline bool send(const char* client, const unsigned char* bytes, size_t length) {
      const IPV4Address addr(client);
      return send(addr, bytes, length);
    }
    bool send(const char* client, const std::string& message) {
      const IPV4Address addr(client);
      return send(addr, message);
    }
    bool send(const char* client, const char* message) {
      const IPV4Address addr(client);
      return send(addr, message);
    }

    template <typename T>
    inline bool send(const char* client, const T& serialized) {
      const IPV4Address addr(client);
      return send(addr, (unsigned char*)&serialized, sizeof(T));
    }
  };
}

#endif