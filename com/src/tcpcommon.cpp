/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 16 Mar 2022
 * Description: Very simple UDP server for C++.
 */

#include <tcpcommon.hpp>

#include <fstream>

namespace TCP
{
  Common::Common(const unsigned short port, const void* env)
    : env(env)
    , mname(".messages." + std::to_string(port) + ".tmp")
    , ename(".errors."   + std::to_string(port) + ".tmp")
    , lastMessage("")
    , lastError("")
  {
    messages = std::fopen(mname.c_str(), "w+");
    errors   = std::fopen(ename.c_str(), "w+");
  }

  Common::~Common() {
    std::fclose(messages);
    std::fclose(errors);
    std::remove(mname.c_str());
    std::remove(ename.c_str());
  }

  const std::string& Common::message() {
    std::fflush(messages);
    {
      std::ifstream mfile(mname, std::ios_base::in);
      std::string   m;

      while (std::getline(mfile, m)) lastMessage = m;
      mfile.close();
    }
    return lastMessage;
  }

  const std::string& Common::error() {
    std::fflush(errors);
    {
      std::ifstream mfile(mname, std::ios_base::in);
      std::string   m;

      while (std::getline(mfile, lastMessage)) lastError = m;
      mfile.close();
    }
    return lastMessage;
  }
}
