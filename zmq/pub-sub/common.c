#include "common.h"

int	 signaled = 0;

void
sighandler(int signo)
{
	signaled = 1;
}

int
s_recv(void *socket, char **ret)
{
	zmq_msg_t	 msg;
	int		 len;

	zmq_msg_init(&msg);
	len = zmq_msg_recv(&msg, socket, 0);
	if (len == -1) {
		zmq_msg_close(&msg);
		return ERROR;
	}

	*ret = calloc(len+1, sizeof(char));
	memcpy(*ret, zmq_msg_data(&msg), len);
	zmq_msg_close(&msg);

	ret[len] = '\x0';
	return OK;
}

int
s_send(void *socket, char *string)
{
	zmq_msg_t	 msg;
	int		 len;

	len = strlen(string);

	zmq_msg_init_size(&msg, len);
	memcpy(zmq_msg_data(&msg), string, len);
	len = zmq_msg_send(&msg, socket, 0);

	zmq_msg_close(&msg);
	return len;
}
