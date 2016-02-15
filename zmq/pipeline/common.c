#include "common.h"

int	 signaled = 0;
int	 timeout = 1000;

void
set_socket_timeout(void *socket)
{
	zmq_setsockopt(
	    socket,
	    ZMQ_RCVTIMEO,
	    &timeout,
	    sizeof(timeout)
	);

	zmq_setsockopt(
	    socket,
	    ZMQ_SNDTIMEO,
	    &timeout,
	    sizeof(timeout)
	);
}

void
sighandler(int signo)
{
	signaled = signo;
}

int
s_recv(void *socket, char **ret)
{
	zmq_msg_t	 msg;
	int		 len;
	int		 i = 0;
	int		 maxtries = 100;

	zmq_msg_init(&msg);

	receiveagain:
	len = zmq_msg_recv(&msg, socket, 0);
	if (len < 0 && errno == EAGAIN && i < maxtries) {
		i++;
		goto receiveagain;
	}
	
	if (len == -1) {
		zmq_msg_close(&msg);
		return len;
	}

	*ret = calloc(len+1, sizeof(char));
	memcpy(*ret, zmq_msg_data(&msg), len);
	zmq_msg_close(&msg);

	return len;
}

int
s_send(void *socket, char *string)
{
	zmq_msg_t	 msg;
	int		 len = 0;
	int		 i = 0;
	int		 maxtries = 100;

	len = strlen(string);

	zmq_msg_init_size(&msg, len);
	memcpy(zmq_msg_data(&msg), string, len);

	sendagain:
	len = zmq_msg_send(&msg, socket, 0);
	if (len < 0 && errno == EAGAIN && i < maxtries) {
		i++;
		goto sendagain;
	}

	zmq_msg_close(&msg);
	return len;
}
