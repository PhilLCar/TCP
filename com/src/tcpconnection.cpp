#include <tcpconnection.hpp>

#include <cstring>

namespace TCP {
  Connection::Connection(TCPConnection *connection, const void* env)
  {
    this->connection = connection;
    this->env        = env;

    connection->env = this;
    tcprecv(connection, receive);
  }

  Connection::~Connection()
  {
    if (connection->callback) {
      tcpover(connection);
    }
  }

  bool Connection::send(const unsigned char* bytes, size_t length) {
    return tcpsend(connection, bytes, length) == (int)length;
  }

  bool Connection::send(const std::string& message) {
    int length = message.length();
    return tcpsend(connection, (unsigned char*)message.c_str(), length) == length;
  }

  bool Connection::send (const char* message) {
    int length = std::strlen(message);
    return tcpsend(connection, (unsigned char*)message, length) == length;
  }

  void Connection::receive(TCPConnection* connection) {
    Connection* con = (Connection*)connection->env;
    Message message;

    message.bytes   = connection->buffer;
    message.size    = connection->bufsize;
    message.address = IPV4Address(ntohl(connection->addr.sin_addr.s_addr), ntohs(connection->addr.sin_port));

    message.bytes[message.size] = 0;
    con->onReceive.trigger(message, con->env);
  }
}