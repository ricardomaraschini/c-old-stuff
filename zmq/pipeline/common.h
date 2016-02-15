#ifndef COMMONH
#define COMMONH
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <proc/readproc.h>
#include <ctype.h>

#define TRUE		1
#define FALSE		0
#define OK		0
#define ERROR		1
#define VENTENDPOINT	"tcp://localhost:5557"
#define VENTBIND	"tcp://*:5557"
#define SINKENDPOINT	"tcp://localhost:5558"
#define SINKBIND	"tcp://*:5558"
#define MQUEUEKEY	(key_t)666
#define MSGSTATUS	1

typedef struct st_msg {
	long int	msgtype;
	long int	count[256];
} stmsg;

int s_recv(void *, char **);
int s_send(void *, char *);
void set_socket_timeout(void *);
void sighandler(int);

#endif
