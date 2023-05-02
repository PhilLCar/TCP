/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP client for C++.
 */

#include <udpclient.hpp>
#include <udpclient.h>

#include <cstring>

namespace UDP
{
  Client::Client(const unsigned short oport, const unsigned short iport,  const char* iaddress, const void* env)
    : Common(oport, env)
  {
    if (oport == iport) {
      // Local connection
      server = udpcopen(iaddress, iport, messages, errors);
    } else {
      // Remote connection
      server = udpiopen(oport, iport, messages, errors);
    }
    server->env = this;
  }

  bool Client::send(const unsigned char* bytes, size_t length) {
    return udpsend(server, bytes, length) == 1;
  }

  bool Client::send(const std::string& message) {
    return udpsend(server, (unsigned char*)message.c_str(), message.length()) == 1;
  }

  bool Client::send (const char* message) {
    return udpsend(server, (unsigned char*)message, std::strlen(message)) == 1;
  }
}
