CC = gcc
CFLAGS = -g -Wall
CLIBS = -lzmq

all:
	$(CC) $(CFLAGS) $(CLIBS) -o server server.c
	$(CC) $(CFLAGS) $(CLIBS) -o client client.c

clean:
	rm -rf server
	rm -rf client
