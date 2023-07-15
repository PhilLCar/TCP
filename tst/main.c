/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 06 Jun 2022
 * Description: Simple test for C.
 */

#include <tcp.h>

#include <stdio.h>
#include <string.h>

typedef struct connection_env {
  char           address[256];
  unsigned short port;
} ConnectionEnv;

typedef struct server_env {
  TCPConnection *connection;
  ConnectionEnv *env;
} ServerEnv;

void onReceive(TCPConnection *connection) {
  ConnectionEnv *cenv = connection->env;

  connection->buffer[connection->bufsize] = 0;
  sprintf(cenv->address, "%s", inet_ntoa(connection->addr.sin_addr));
  cenv->port = ntohs(connection->addr.sin_port);
  printf("From %s(%hu): %s\n", (char*)connection->env, cenv->port, connection->buffer);
}

void onConnect(TCPServer *server, int client) {
  ServerEnv *senv = server->env;
  senv->connection = server->connections[client];
  senv->connection->env = senv->env;
  inet_aton(senv->env->address, &senv->connection->addr.sin_addr);
  senv->env->port = htons(senv->connection->addr.sin_port);
  tcprecv(senv->connection, onReceive);
}

int main(int argc, char *argv[]) {
  TCPServer     *server = NULL;
  ConnectionEnv  cenv   = { "127.0.0.1", 8008 };
  ServerEnv      senv   = { NULL, &cenv };

  if (argc > 1) {
    if (!strcmp(argv[1], "-s")) {
      server = tcpopen(cenv.port, stdout, stderr);
      if (server) {
        server->env = &senv;
        tcpaccept(server, onConnect);
      }
    } else if (!strcmp(argv[1], "-c")) {
      senv.connection = tcpconnect(cenv.address, cenv.port, stdout, stderr);
      if (senv.connection)
      {
        senv.connection->env = &cenv;
        tcprecv(senv.connection, onReceive);
      }
    }

    printf("Send 'q' to quit.\n");

    while (1) {
      const size_t size = 1024;
      char         line[size];
      char        *lineptr = line;

      getline((char**)&lineptr, (size_t*)&size, stdin);
      if (senv.connection && !senv.connection->callback) break;
      if (strcmp(line, "q\n")) {
        if (senv.connection) {
          // -1 because don't send newline char
          tcpsend(senv.connection, (unsigned char*)line, strlen(line) - 1);
        } else {
          fprintf(stderr, "Cannot send a message because no client has sent anything yet.\n");
        }
      } else break;
    }
    if (senv.connection) tcpover(senv.connection);
    if (server)          tcpclose(server);
  } else {
    printf("Run '%s -c' for a client, and '%s -s' for a server\n", argv[0], argv[0]);
  }

  return 0;
}