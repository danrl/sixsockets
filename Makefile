CC	=	gcc
CFLAGS	+=	-std=c99 -Wall -pedantic
LDFLAGS	+=

all: addrsrv6 bannergrab6

addrsrv6: addrsrv6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

bannergrab6: bannergrab6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -rf	addrsrv6 \
		addrsrv6.o
	rm -rf	bannergrab6 \
		bannergrab6.o

.PHONY: clean
