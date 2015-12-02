/*
 * clientTCP.c: a stream socket client for a simple chat
 */

#include "tcputils.h"

int main(int argc, char *argv[])
{
    // get tcp socket file descriptor (fd) using server IP address
    int sockfd;
    if (argc != 2) {
        printf("Using localhost. You can use: %s ipaddress\n", argv[0]);
        sockfd = get_socket_tcp("127.0.0.1");
    } else {
        sockfd = get_socket_tcp(argv[1]);
    }

    fd_set read_fds; // set of fds used in function select()
    uint8_t stdin_fd = STDIN_FILENO; // store fd for stdin
    uint8_t nbytes;
    uint8_t msg_len;
    char ch;
    char txtbuf[MSGSIZE];

    printf("Start chatting\n");

    while (1) {
        // clear read_fds and add fds for stdin and tcp socket
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        FD_SET(sockfd, &read_fds);

        // monitor fds
        if (select(sockfd + 1, &read_fds, NULL, NULL, NULL) < 0)
            perror("select()");

        // recv()'d something from the server
        if (FD_ISSET(sockfd, &read_fds)) {
            memset(txtbuf, '\0', MSGSIZE);

            if ((nbytes = recv(sockfd, txtbuf, MSGSIZE - 1, 0)) < 0) {
                perror("recv()");
                return -1;
            }

            if (nbytes == 0) { // connection closed
                printf("Server disconnected\n");
                return 1;
            }

            printf("> '%s'\n", txtbuf);
        }

        // user is typing a message
        if (FD_ISSET(stdin_fd, &read_fds)) {

            memset(txtbuf, '\0', MSGSIZE);
            if (fgets(txtbuf, MSGSIZE, stdin) == NULL)
                perror("fgets()");

            // get rid of unnecessary characters
            txtbuf[strcspn(txtbuf, "\r\n")] = '\0';
            msg_len = strlen(txtbuf);

            // clear the stdin buffer
            if (msg_len == MSGSIZE - 1)
                while ((ch = getchar()) != '\n' && ch != EOF);

            if (send_full(sockfd, txtbuf, msg_len, 0) < 0)
                perror("send_full()");
        }
    }

    close(sockfd);

    return 0;
}
