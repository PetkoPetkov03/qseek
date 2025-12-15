#ifndef __HTTP_DNS__
#define __HTTP_DNS__

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

inline static char* htoip(char* host) {
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];
    char *result = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, NULL, &hints, &res) != 0)
        return NULL;

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;

        if (p->ai_family == AF_INET) {
            addr = &((struct sockaddr_in*)p->ai_addr)->sin_addr;
        } else if (p->ai_family == AF_INET6) {
            addr = &((struct sockaddr_in6*)p->ai_addr)->sin6_addr;
        } else {
            continue;
        }

        if (inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr)) == NULL)
            continue;

        result = strdup(ipstr);
        break;
    }

    freeaddrinfo(res);
    return result;
}

#endif
