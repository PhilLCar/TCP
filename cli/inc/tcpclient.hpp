/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP client for C++.
 */

#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <common.hpp>

#include <string>

namespace UDP {
  class Client : public Common {
  public:
    inline Client(const unsigned short port, const char* address)
      : Client(port, port, address, nullptr)
    {}
    inline Client(const unsigned short oport, const unsigned short iport)
      : Client(oport, iport, "127.0.0.1", nullptr)
    {}

    template <typename T>
    inline Client(const unsigned short port, const char* address, const T& env)
      : Client(port, port, address, (const void*)&env)
    {}
    template <typename T>
    inline Client(const unsigned short oport, const unsigned short iport, const T& env)
      : Client(oport, iport, "127.0.0.1", (const void*)&env)
    {}

    ~Client();

  private:
    explicit Client(const unsigned short oport, const unsigned short iport, const char* iaddress, const void* env);

  public:
    bool send(const unsigned char* bytes, size_t length);
    bool send(const std::string& message);
    bool send(const char* message);

    template <typename T>
    inline bool send(const T& serialized) {
      return send((unsigned char*)&serialized, sizeof(T));
    }
  };
}

#endif