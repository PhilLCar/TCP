/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Common structures for C++.
 */

#ifndef COMMON_HPP
#define COMMON_HPP

#include <common.h>

#include <vector>
#include <algorithm>
#include <string>

namespace UDP {
  class IPV4Address;
}

namespace std {
  string to_string(UDP::IPV4Address address);
}

namespace UDP {
  struct IPV4Address {
  public:
    IPV4Address(const char *address);
    IPV4Address(const unsigned int address, const unsigned short port);

    inline IPV4Address(const std::string& address) : IPV4Address(address.c_str()) {}
    inline IPV4Address() : IPV4Address(0, 0) {}

  public:
    inline const std::string&   address_str() const { return addr_str; }
    inline       unsigned int   address_int() const { return addr_int; }
    inline       unsigned short port()        const { return _port;    }

    inline bool operator ==(IPV4Address& other) {
      return addr_int == other.addr_int && _port == other._port;
    }

  public:
    friend std::string std::to_string(IPV4Address address);

  private:
    union {
      unsigned char bytes[4];
      unsigned int  addr_int;
    };
    unsigned short _port;
    std::string    addr_str;
  };

  struct Message {
    unsigned char* bytes;
    size_t         size;
    IPV4Address    address;
  };

  typedef void (*Callback)(Message&, void*);

  class ReceptionEvent {
    friend class Common;

  public:
    inline void operator +=(Callback callback) {
      callbacks.push_back(callback);
    }
    inline void operator -=(Callback callback) {
      callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

  private:
    inline void trigger(Message& message, const void *env) {
      for (auto callback : callbacks) callback(message, (void*)env);
    }

  private:
    std::vector<Callback> callbacks;
  };

  class Common {
  protected:
    explicit Common(const unsigned short port, const void *env);

  public:
    ~Common();

  public:
    void start();
    void stop();

    const std::string& message();
    const std::string& error();

  private:
    static void receive(UDPServer* server);

  public:
    ReceptionEvent onReceive;

  protected:
    const unsigned short port;
    const void*          env;

    UDPServer*  server;
    std::string mname;
    std::string ename;
    std::string lastMessage;
    std::string lastError;
    std::FILE*  messages;
    std::FILE*  errors;
  };
}

#endif