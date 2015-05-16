CC	=	gcc
CFLAGS	+=	-std=c99 -Wall -pedantic
LDFLAGS	+=

all:	addrsrv6 \
	bannergrab6 \
	oracle6 \
	wisdomsrv6 \
	mcsnoop6 \
	flowgen6

addrsrv6: addrsrv6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

bannergrab6: bannergrab6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

oracle6: oracle6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

wisdomsrv6: wisdomsrv6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

mcsnoop6: mcsnoop6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

flowgen6: flowgen6.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<


clean:
	rm -rf	addrsrv6 \
		addrsrv6.o
	rm -rf	bannergrab6 \
		bannergrab6.o
	rm -rf	oracle6 \
		oracle6.o
	rm -rf	wisdomsrv6 \
		wisdomsrv6.o
	rm -rf	mcsnoop6 \
		mcsnoop6.o
	rm -rf	flowgen6 \
		flowgen6.o

.PHONY: clean
