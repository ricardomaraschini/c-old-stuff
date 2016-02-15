CC = gcc
CFLAGS = -g -Wall
CLIBS = -lzmq

all:
	$(CC) $(CFLAGS) $(CLIBS) -o subscriber subscriber.c common.c
	$(CC) $(CFLAGS) $(CLIBS) -o publisher publisher.c common.c

clean:
	rm -rf subscriber
	rm -rf publisher
