#include "tcputils.h"

/*
 * Function: init_socket_tcp
 * -----------------------------
 * initializes a tcp socket for a server
 */
int init_socket_tcp()
{
    struct addrinfo hints, *ai, *p;
    memset(&hints, 0, sizeof(hints));
    //hints.ai_flags    = AI_NUMERICHOST | AI_NUMERICSERV;
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(NULL, PORTNUM, &hints, &ai) != 0) {
        perror("getaddrinfo()");
        exit(1);
    }

    int sockfd, yes = 1;
    for (p = ai; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) continue;

        // avoid "address already in use" error message
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(sockfd);
            continue;
        }

        break;
    }

    // if p is NULL => did not bind
    if (p == NULL) {
        fprintf(stderr, "selectserver : failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai);

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen()");
        exit(3);
    }

    return sockfd;
}

/*
 * Function: get_socket_tcp
 * ------------------------
 * initializes a tcp socket for a client
 */
int get_socket_tcp(char *servip)
{
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    //hints.ai_flags    = AI_NUMERICHOST | AI_NUMERICSERV;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(servip, PORTNUM, &hints, &servinfo) != 0) {
        perror("client: getaddrinfo()");
        return 1;
    }

    int sockfd;
    /** loop through all the results and connect to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("socket()");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(sockfd);
            perror("connect()");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof(s));
    printf("client: connection to %s\n", s);

    freeaddrinfo(servinfo);

    return sockfd;
}

/* Function: get_in_addr
 * ---------------------
 * returns sockaddr structure, depending on AF_INET (IPv4 or IPv6)
 * to be use in function inet_ntop()
 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
 * Function: send_full
 * -------------------
 *  attempts to send the entire data in chunks if function
 *  send() did not succeed to send everything in a single run
 */
int send_full(int s, char *buf, int len, int sflag)
{
    int n, total = 0, left = len;

    while (total < len) {
        if ((n = send(s, buf + total, left, sflag)) < 0) break;
        total += n;
        left  -= n;
    }

    return n < 0 ? -1 : 0;
}
