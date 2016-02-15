#ifndef XCOMMONH
#define XCOMMONH
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <assert.h>
#include <signal.h>

#define TRUE	1
#define FALSE	0
#define OK	0
#define ERROR	1

int s_recv(void *, char **);
int s_send(void *, char *);
void sighandler(int);

#endif
