#include "common.h"

extern int	 signaled;
stmsg		 stats;

void
clean_stats()
{
	int	 i;
	for(i=0; i<256; i++)
		stats.count[i] = 0;
}

void
write_stats()
{
	int	 msgid;
	int	 msglen;

	msgid = msgget(MQUEUEKEY, 0666|IPC_CREAT);
	if (msgid == -1) {
		printf("unable to create message queue");
		return;
	}

	msglen = sizeof(stats) - sizeof(long);
	if (msgsnd(msgid, (void *)&stats, msglen, IPC_NOWAIT) == -1) {
		printf("unable to send message");
		return;
	}

	signaled = 0;
}

int
main(void)
{
	void	*context = NULL;
	void	*receiver = NULL;
	char	*msg = NULL;
	char	*piece = NULL;
	int	 idx = 0;

	stats.msgtype = MSGSTATUS;
	signal(SIGUSR1, sighandler);
	signal(SIGUSR2, sighandler);
	context = zmq_ctx_new();
	receiver = zmq_socket(context, ZMQ_PULL);
	set_socket_timeout(receiver);
	zmq_bind(receiver, SINKBIND);

	while(TRUE) {
		if (signaled) {
			if (signaled == SIGUSR1)
				write_stats();
			else if (signaled == SIGUSR2)
				clean_stats();
			else
				break;
		}

		if (s_recv(receiver, &msg) < 0) {
			perror("s_recv");
			continue;
		}

		piece = strtok(msg, "\t");
		idx = atoi(piece);
		piece = strtok(NULL, "\t");
		stats.count[idx] += atoi(piece);
		free(msg);
	}

	zmq_close(receiver);
	zmq_ctx_destroy(context);

	return OK;
}
