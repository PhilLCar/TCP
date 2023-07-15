/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 26 May 2022
 * Description: Common structures for C.
 */

#ifndef TCPCOMMON_H
#define TCPCOMMON_H

#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/time.h>

// With those settings (128 connections with a buffer of 65536 each), running the server will require to allocate
// exactly 8Mb of stack space in buffers, which is reasonable given the intention to use this on microcontrollers.
// The choice to avoid mallocs was made to minimize the risk over memory leaks and/or runtime allocation failures.
#define TCP_MAX_CHUNK (1 << 16)
#define TCP_MAX_CONN        128
#define TCP_TIMEOUT        3000

extern const struct timeval TIMEOUT;

/*
NOTE:
This isn't made to be thread safe for multiple sending threads. If multiple
threads are going to send messages, they should make sure between themselves
that none are sending simultaneously.
*/

typedef struct tcp_connection {
  FILE                   *messages;
  FILE                   *errors;
  int                     fd;
  unsigned char           buffer[TCP_MAX_CHUNK];
  int                     bufsize;
  struct sockaddr_in      addr;
  pthread_t               thread;
  void                  (*callback)(struct tcp_connection*);
  void                   *env;
  struct tcp_connection **self;
} TCPConnection;

typedef struct tcp_server {
  FILE               *messages;
  FILE               *errors;
  int                 fd;
  struct sockaddr_in  addr;
  pthread_t           thread;
  void              (*callback)(struct tcp_server*, int);
  TCPConnection      *connections[TCP_MAX_CONN];
  void               *env;
} TCPServer;

typedef void (*TCPConnCallback)(TCPServer *server, int client);
typedef void (*TCPRecvCallback)(TCPConnection *connection);

#ifdef __cplusplus
extern "C" {
#endif

void tcprecv(TCPConnection *connection, TCPRecvCallback callback);
int  tcpsend(TCPConnection *connection, const unsigned char *bytes, size_t length);
void tcpover(TCPConnection *connection);


#ifdef __cplusplus
}
#endif

#endif