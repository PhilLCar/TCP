/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 26 May 2022
 * Description: Common structures for C.
 */

#include <tcpcommon.h>

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

const struct timeval TIMEOUT = { TCP_TIMEOUT / 1000, (TCP_TIMEOUT % 1000) * 1000 };

void *_tcprecv(void *vargp) {
  TCPConnection *connection = (TCPConnection*)vargp;

  while (connection->callback) {
    int n = recv(connection->fd, connection->buffer, TCP_MAX_CHUNK, 0);
    if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      fprintf(connection->errors, "Couldn't receive!\n");
      connection->callback = NULL;
    } else if (n > 0 && connection->callback) {
      connection->bufsize = n;
      connection->callback(connection);
    }
  }

  return NULL;
}

void tcprecv(TCPConnection *connection, TCPRecvCallback callback) {
  connection->callback = callback;
  pthread_create(&connection->thread, NULL, _tcprecv, connection);
}

int tcpsend(TCPConnection *connection, const unsigned char *bytes, size_t length) {
  return send(connection->fd, bytes, length, 0);
}

void tcpover(TCPConnection *connection) {
  connection->callback = NULL;
  pthread_join(connection->thread, NULL);
  close(connection->fd);
  if (connection->self) *connection->self = NULL;
  free(connection);
}