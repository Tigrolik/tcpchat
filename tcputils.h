#ifndef _TCPUTILS_H_
#define _TCPUTILS_H_

#include "common.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORTNUM "6534"
#define MSGSIZE 256
#define BACKLOG 10

/*
 * Function: int init_socket_tcp
 * initializes a tcp socket for a server
 *
 * @return: socket file descriptor
 */
int init_socket_tcp();

/*
 * Function: get_socket_tcp
 * initializes a tcp socket for a client
 *
 * @args: server ip address in string format
 *
 * @return: socket file descriptor
 */
int get_socket_tcp(char*);

/* Function: get_in_addr
 * ---------------------
 * returns sockaddr structure, depending on AF_INET (IPv4 or IPv6)
 * to be used in function inet_ntop()
 *
 * @args: sockaddr structure of dest
 */
void *get_in_addr(struct sockaddr*);

/*
 * Function: send_full
 * -------------------
 *  attempts to send the entire data in chunks if function
 *  send() did not succeed to send everything in a single run
 *
 *  @args: the same af for function send()
 *
 *  @return: -1 on failure, 0 on success
 */
int send_full(int, char*, int, int);

#endif
