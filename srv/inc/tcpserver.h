/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 06 Jun 2022
 * Description: Very simple TCP server for C.
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <tcpcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

TCPServer *tcpopen(const unsigned short port, FILE *messages, FILE *errors);
void       tcpclose(TCPServer *server);

void tcpaccept(TCPServer *server, TCPConnCallback callback);
void tcpcancel(TCPServer *server);

#ifdef __cplusplus
}
#endif

#endif