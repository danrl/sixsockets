/*
 * Copyright (c) 2013, Dan Luedtke <mail@danrl.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVERPORT      1337
#define LISTENBACKLOG   5


const char *progname = "addrsrv6";
const char *progdate = "June 2013";


int main (int __attribute__((unused)) argc,
          char __attribute__((unused)) *argv[])
{
    int sd, sdc;
    struct sockaddr_in6 sa, sac;
    socklen_t sac_size = sizeof(sac);
    char address[INET6_ADDRSTRLEN] = { 0x0 };
    char buffer[INET6_ADDRSTRLEN + 9] = { 0x0 }; /* [addr]:port\n */

    /* initialize sockaddr */
    memset(&sa, 0x0, sizeof(sa));
    sa.sin6_family = AF_INET6;
    sa.sin6_port = htons(SERVERPORT);
    /*
     * note: in6addr_any refers to :: and :: is all zeros. since we used
     * memset to zero the whole struct sockaddr_in6 everything is fine.
     * one could, of course, explicitly set the address:
     * memcpy(sa.sin6_addr, in6addr_any, sizeof(sin6_addr));
     */

    /* create tcp socket */
    if ((sd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "error: socket: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* bind socket */
    if (bind(sd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
        fprintf(stderr, "error: bind: %s\n", strerror(errno));
        close(sd);
        return EXIT_FAILURE;
    }

    /* turn active socket into listening socket */
    if (listen(sd, LISTENBACKLOG) < 0) {
        fprintf(stderr, "error: listen: %s\n", strerror(errno));
        close(sd);
        return EXIT_FAILURE;
    }

    for (;;) {
        /* accept new connections */
        memset(&sac, 0x0, sizeof(sac));
        if ((sdc = accept(sd, (struct sockaddr *) &sac, &sac_size)) < 0) {
            fprintf(stderr, "error: accept: %s\n", strerror(errno));
            close(sd);
            return EXIT_FAILURE;
        }

        /* get presentation of client address */
        inet_ntop(AF_INET6, &sac.sin6_addr, address, sizeof(address));
        snprintf(buffer, sizeof(buffer), "[%s]:%d\n", address,
                 ntohs(sac.sin6_port));
        printf("accept: %s", buffer);

        /* send address back to client */
        write(sdc, buffer, sizeof(buffer));

        /* close client socket */
        close(sdc);
        printf("close:  %s", buffer);
    }


    /* close socket */
    close(sd);
    return EXIT_SUCCESS;
}
