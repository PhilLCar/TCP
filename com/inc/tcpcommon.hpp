/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Common structures for C++.
 */

#ifndef TCPCOMMON_HPP
#define TCPCOMMON_HPP

#include <tcpcommon.h>

#include <string>

namespace TCP {
  class Common {
  protected:
    explicit Common(const unsigned short port, const void* env);

  public:
    ~Common();

  public:
    const std::string& message();
    const std::string& error();

  protected:
    const void* env;

    std::string mname;
    std::string ename;
    std::string lastMessage;
    std::string lastError;
    std::FILE*  messages;
    std::FILE*  errors;
  };
}

#endif