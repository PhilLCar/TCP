/* Author: Philippe Caron (philippe-caron@hotmail.com)
 * Date: 06 Jun 2022
 * Description: Very simple UDP client for C.
 */

#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

TCPConnection *tcpconnect(const char *address, const unsigned short port, FILE *messages, FILE *errors);

#ifdef __cplusplus
}
#endif

#endif