#include <stdio.h>
#include <zmq.h>
#include <unistd.h>
#include <string.h>

int
main(void)
{
	void		*context;
	void		*requester;
	zmq_msg_t	 out_msg;
	zmq_msg_t	 in_msg;
	char		 response[6];

	memset(response, '\x0', 6);

	context = zmq_ctx_new();
	requester = zmq_socket(context, ZMQ_REQ);
	zmq_connect(requester, "tcp://localhost:5555");
	
	zmq_msg_init_size(&out_msg, 5);
	memcpy(zmq_msg_data(&out_msg), "Hello", 5);
	printf("sending Hello...\n");
	zmq_msg_send(&out_msg, requester, 0);
	printf("Hello sent, awayting for response...\n");
	zmq_msg_close(&out_msg);

	zmq_msg_init(&in_msg);
	zmq_msg_recv(&in_msg, requester, 0);
	strncpy(response, zmq_msg_data(&in_msg), 5);
	zmq_msg_close(&in_msg);

	printf("received '%s'\n", response);
	zmq_close(requester);
	zmq_ctx_destroy(context);
	return 0;
}
