/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP client for C++.
 */

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <tcpcommon.hpp>
#include <tcpconnection.hpp>

#include <string>

using namespace Networking;

namespace TCP {
  class Client : public Common {
  public:
    inline Client(const IPV4Address address)
      : Client(address.address_str().c_str(), address.port(), nullptr)
    {}
    inline Client(const unsigned short port)
      : Client("127.0.0.1", port, nullptr)
    {}
    inline Client(const char* address, const unsigned short port)
      : Client(address, port, nullptr)
    {}

    template <typename T>
    inline Client(const IPV4Address address, const T& env)
      : Client(address.address_str().c_str(), address.port(), (const void*)&env)
    {}
    template <typename T>
    inline Client(const unsigned short port, const T& env)
      : Client("127.0.0.1", port, (const void*)&env)
    {}
    template <typename T>
    inline Client(const char* address, const unsigned short port, const T& env)
      : Client(address, port, (const void*)&env)
    {}

  private:
    explicit Client(const char* iaddress, const unsigned short port, const void* env);

  public:
    inline Connection& getConnection() { return connection; }

  private:
    Connection connection;
  };
}

#endif