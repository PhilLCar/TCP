/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 06 Jun 2022
 * Description: Very simple TCP client for C.
 */

#include <tcpclient.h>

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

int _tcpconnect(struct sockaddr_in *addr, FILE *messages, FILE *errors) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(socket_fd < 0){
    fprintf(errors, "Couldn't create socket!\n");
    return socket_fd;
  }
  fprintf(messages, "Socket created successfully.\n");

  if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &TIMEOUT, sizeof(struct timeval)) < 0) {
    fprintf(errors, "Couldn't set timeout!\n");
  }
  
  if(connect(socket_fd, (struct sockaddr*)addr, sizeof(struct sockaddr_in))) {
    fprintf(errors, "Connection failed!\n");
    close(socket_fd);
    return -1;;
  }
  fprintf(messages, "Connected successfully.\n");

  return socket_fd;
}


TCPConnection *tcpconnect(const char *address, const unsigned short port, FILE *messages, FILE *errors) {
  struct sockaddr_in  addr;
  int                 socket_fd;
  TCPConnection      *connection = NULL;

  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if ((socket_fd = _tcpconnect(&addr, messages, errors))) {
    connection           = malloc(sizeof(TCPConnection));
    connection->messages = messages;
    connection->errors   = errors;
    connection->fd       = socket_fd;
    connection->addr     = addr;
    connection->callback = NULL;
    connection->self     = NULL;
  }
  
  return connection;
}