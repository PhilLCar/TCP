/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 06 Jun 2022
 * Description: Very simple TCP server for C.
 */

#include <tcpserver.h>

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

TCPServer *tcpopen(const unsigned short port, FILE *messages, FILE *errors) {
  struct sockaddr_in  addr;
  int                 socket_fd;
  TCPServer          *server = NULL;
  
  socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  if(socket_fd < 0){
    fprintf(errors, "Couldn't create socket!\n");
    return server;
  }
  fprintf(messages, "Socket created successfully.\n");
  
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &TIMEOUT, sizeof(struct timeval)) < 0) {
    fprintf(errors, "Couldn't set timeout!\n");
  }
  
  if(bind(socket_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
    fprintf(errors, "Bind failed!\n");
    close(socket_fd);
    return server;
  }
  fprintf(messages, "Binded successfully.\n");

  listen(socket_fd, TCP_MAX_CONN);
  
  server           = malloc(sizeof(TCPServer));
  server->messages = messages;
  server->errors   = errors;
  server->fd       = socket_fd;
  server->addr     = addr;
  server->callback = NULL;
  server->env      = NULL;
  
  return server;
}

void tcpclose(TCPServer *server) {
  if (server->callback) tcpcancel(server);
  close(server->fd);
  free(server);
}

void *_tcpaccept(void *vargp) {
  struct sockaddr_in  addr;
  socklen_t           addr_size = sizeof(struct sockaddr_in);
  TCPServer          *server    = (TCPServer*)vargp;

  while (server->callback) {
    int connfd = accept(server->fd, (struct sockaddr*)&addr, &addr_size);
    if (connfd < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      fprintf(server->errors, "Couldn't accept!\n");
      server->callback = NULL;
    } else if (connfd > 0 && server->callback) {
      int client = -1;
      for (int i = 0; i < TCP_MAX_CONN; i++) {
        if (!server->connections[i]) {
          TCPConnection *connection =  malloc(sizeof(TCPConnection));
          if (connection) {
            connection->messages   = stdout; // default
            connection->errors     = stderr; // default
            connection->fd         = connfd;
            memset(connection->buffer, 0, TCP_MAX_CHUNK);
            connection->bufsize    = 0;
            connection->addr       = addr;
            connection->callback   = NULL;
            connection->env        = NULL;
            connection->self       = &server->connections[i];
            server->connections[i] = connection;
            client                 = i;
            break;
          }
        }
      }
      if (client >= 0) {
        server->callback(server, client);
      } else {
        fprintf(server->messages, "Maximum number of connections reached! (%d)\n", TCP_MAX_CONN);
      }
    }
  }

  // Trigger all the active connections to close
  for (int i = 0; i < TCP_MAX_CONN; i++) {
    TCPConnection *connection = server->connections[i];
    if (connection) {
      tcpover(connection);
      server->connections[i] = NULL;
    }
  }

  return NULL;
}

void tcpaccept(TCPServer *server, TCPConnCallback callback) {
  server->callback = callback;
  pthread_create(&server->thread, NULL, _tcpaccept, server);
}

void tcpcancel(TCPServer *server) {
  server->callback = NULL;
  pthread_join(server->thread, NULL);
}