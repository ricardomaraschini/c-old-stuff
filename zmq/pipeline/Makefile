CC = gcc
CFLAGS = -g -Wall
CLIBS = -lzmq -lproc

all:
	$(CC) $(CFLAGS) $(CLIBS) -o ventilator ventilator.c common.c
	$(CC) $(CFLAGS) $(CLIBS) -o worker worker.c common.c
	$(CC) $(CFLAGS) $(CLIBS) -o sink sink.c common.c
	$(CC) $(CFLAGS) $(CLIBS) -o stats stats.c common.c

clean:
	rm -rf ventilator
	rm -rf worker
	rm -rf sink
	rm -rf stats
