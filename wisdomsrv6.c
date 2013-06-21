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

#define	SERVERPORT	1337

const char *progname = "wisdomsrv6";
const char *progdate = "June 2013";
const char *wisdom = "The words of the prophets\n"			\
		     "are written on the subway walls,\n"		\
		     "and tenement halls,\n"				\
		     "and whispered in the sound of silence.\n";

int main (int argc, char *argv[])
{
	int sd;
	struct sockaddr_in6 sa, sac;
	socklen_t sac_size = sizeof(sac);
	char address[INET6_ADDRSTRLEN] = { 0x0 };

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

	/* create udp socket */
	if ((sd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "error: socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	/* bind socket */
	if (bind(sd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		fprintf(stderr, "error: bind: %s\n", strerror(errno));
		close(sd);
		return EXIT_FAILURE;
	}

	for (;;) {
		/* act upon incoming datagrams */
		if (recvfrom(sd, NULL, 0, 0, (struct sockaddr *) &sac,
			     &sac_size) < 0) {
			fprintf(stderr, "error: recvfrom: %s\n",
				strerror(errno));
			close(sd);
			return EXIT_FAILURE;
		}

		/* get presentation of client address */
		inet_ntop(AF_INET6, &sac.sin6_addr, address, sizeof(address));
		printf("datagram from: [%s]:%d\n", address,
		       ntohs(sac.sin6_port));

		/* send the client some wisdom */
		if (sendto(sd, wisdom, strlen(wisdom), 0,
			   (struct sockaddr *) &sac,sizeof(sac)) < 0) {
			fprintf(stderr, "error: sendto: %s\n", strerror(errno));
			close(sd);
			return EXIT_FAILURE;
		}
	}

	/* close socket */
	close(sd);
	return EXIT_SUCCESS;
}
