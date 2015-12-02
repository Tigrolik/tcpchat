/*
 * serverTCP.c: simple chat server
 */

#include "tcputils.h"
#include "sigutils.h"

int main(void)
{
    // initialize tcp socket file descriptor (fd)
    int sockfd = init_socket_tcp();

    // take care of dead processes
    sigchld_handler();

    printf("Server: waiting for connections...\n");

    fd_set master; // master fds for select()
    fd_set read_fds; // temp fds for select()
    uint8_t newfd; // temp fd for clients (incoming connections)
    uint8_t fdmax = sockfd; // keep track of the max fd
    uint8_t i, j, nbytes;
    char buf[MSGSIZE], remoteIP[INET6_ADDRSTRLEN];
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;

    // clear sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    // add the sockfd to the master set
    FD_SET(sockfd, &master);

    while (1) {
        read_fds = master;
        // monitor fds
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select()");
            return -1;
        }

        // go through existing connections looking for data to read
        for (i = 0; i <= fdmax; i++)
            if (FD_ISSET(i, &read_fds)) {
                memset(buf, '\0', MSGSIZE);
                // handle new connections
                if (i == sockfd) {
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(sockfd, (struct sockaddr *)&remoteaddr,
                                    &addrlen)) < 0) {
                        perror("accept()");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) fdmax = newfd;
                        printf("Selectserver: new connection from %s on socket"
                                " %d\n", inet_ntop(remoteaddr.ss_family,
                                    get_in_addr((struct sockaddr*)&remoteaddr),
                                    remoteIP, INET6_ADDRSTRLEN), newfd);
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, MSGSIZE - 1, 0)) > 0) {
                        printf("Socket %d message: %s\n", i, buf);
                        // got data from a client
                        for (j = 0; j <= fdmax; j++)
                            // send to everyone
                            if (FD_ISSET(j, &master) && j != sockfd && j != i)
                                // except the sockfd and ourselves
                                if (send_full(j, buf, nbytes, 0) < 0)
                                    perror("send()");
                    } else { // connection closed
                        if (nbytes == 0) {
                            printf("selectserver : socket %d hung up\n", i);
                        } else {
                            perror("recv()");
                        }
                        close(i);
                        FD_CLR(i, &master); // remove from master set
                    }
                }
            }
    }
    return 0;
}
