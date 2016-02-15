#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <zmq.h>

int	 signaled;

void
sighandler(int signo)
{
	signaled = 1;
}

int
main(void)
{
	void		*context;
	void		*responder;
	zmq_msg_t	 in_msg;
	zmq_msg_t	 out_msg;

	signal(SIGTERM, sighandler);
	context = zmq_ctx_new();
	responder = zmq_socket(context, ZMQ_REP);
	zmq_bind(responder, "tcp://*:5555");

	while(1) {
		if (signaled)
			break;

		zmq_msg_init(&in_msg);
		zmq_msg_recv(&in_msg, responder, 0);
		zmq_msg_close(&in_msg);

		// do something useful
		sleep(5);

		zmq_msg_init_size(&out_msg, 5);
		memcpy(zmq_msg_data(&out_msg), "World", 5);
		zmq_msg_send(&out_msg, responder, ZMQ_DONTWAIT);
		zmq_msg_close(&out_msg);	
	}

	zmq_close(responder);
	zmq_ctx_destroy(context);

	return 0;
}
