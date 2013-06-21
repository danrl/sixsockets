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

const char *progname = "mcsnoop6";
const char *progdate = "June 2013";

int main (int argc, char *argv[])
{
	int sd;
	struct sockaddr_in6 sa;
	socklen_t sa_size = sizeof(sa);
	char buffer[1024] = { 0x0 };
	char address[INET6_ADDRSTRLEN] = { 0x0 };

	/* create raw socket */
	if ((sd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0) {
		fprintf(stderr, "error: socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	/*
	 * further filtering von icmpv6 messages can be achieved through
	 * the advanced sockets API for IPv6
	 * https://tools.ietf.org/html/rfc2292#section-3.2
	 */

	for (;;) {
		/* receive data */
		int len;
		len = recvfrom(sd, buffer, sizeof(buffer) - 1, 0,
			       (struct sockaddr *) &sa, &sa_size);

		/* get presentation of sender address */
		inet_ntop(AF_INET6, &sa.sin6_addr, address, sizeof(address));

		/* print data pretending it is a string */
		printf("received %d bytes from %s\n", len, address);
		for (int i = 0; i < len; i++)
			printf("%02x", buffer[i] & 0xff);
		printf("\n");
	}

	/* close socket */
	close(sd);
	return EXIT_SUCCESS;
}

