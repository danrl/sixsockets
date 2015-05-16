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
#include <arpa/inet.h>
#include <linux/in6.h>
#include <sys/socket.h>
#include <time.h>       /* to initialize srand() */


const char *progname = "flowgen6";
const char *progdate = "May 2015";


int main (int argc, char *argv[])
{
    int sd;
    int port;
    struct sockaddr_in6 sa;
    int i;
    const int on = 1;
    char address[INET6_ADDRSTRLEN] = { 0x0 };

    /* print usage information on invocation error */
    if (argc != 3) {
        fprintf(stderr, "%s %s\n", progname, progdate);
        fprintf(stderr, "usage: %s <ipv6 address> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* initialize sockaddr */
    memset(&sa, 0x0, sizeof(sa));
    sa.sin6_family = AF_INET6;

    /* check and set address */
    if (inet_pton(AF_INET6, argv[1], &sa.sin6_addr) != 1) {
        fprintf(stderr, "error: invalid address\n");
        return EXIT_FAILURE;
    }

    /* check and set port */
    port = atoi(argv[2]);
    if (port < 1 || port > 1<<16) {
        fprintf(stderr, "error: invalid port\n");
        return EXIT_FAILURE;
    }
    sa.sin6_port = htons(port);


    /* create udp socket */
    if ((sd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "error: socket: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* set autoflowlabel */
    if (setsockopt(sd, SOL_IPV6, IPV6_AUTOFLOWLABEL, (void *) &on, sizeof(on)) == -1) {
        fprintf(stderr, "error: setsockopt: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* start 'connection' */
    if (connect(sd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
        fprintf(stderr, "error: connect: %s\n", strerror(errno));
        close(sd);
        return EXIT_FAILURE;
    }

    /* send 3 datagrams */
    inet_ntop(AF_INET6, &sa.sin6_addr, address, sizeof(address));
    for (i = 0; i < 3; i++) {
        /* send empty datagram */
        printf("sending datagram to [%s]:%d\n", address, ntohs(sa.sin6_port));
        if (sendto(sd, NULL, 0, 0, NULL, 0) < 0) {
            fprintf(stderr, "error: sendto: %s\n", strerror(errno));
            close(sd);
            return EXIT_FAILURE;
        }
        sleep(1);
    }

    /* close socket */
    close(sd);
    return EXIT_SUCCESS;
}
