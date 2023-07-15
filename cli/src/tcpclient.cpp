/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP client for C++.
 */

#include <tcpclient.hpp>
#include <tcpclient.h>

#include <cstring>

namespace TCP
{
  Client::Client(const char *address, const unsigned short port, const void* env)
    : Common(port, env)
    , connection(tcpconnect(address, port, messages, errors), env)
  {}
}
