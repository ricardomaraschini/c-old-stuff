#include "common.h"

extern int	 signaled;

int
main(void)
{
	void		*context = NULL;
	void		*publisher = NULL;
	char		*str = NULL;
	int		 rc = OK;

	signal(SIGTERM, sighandler);
	context = zmq_ctx_new();
	publisher = zmq_socket(context, ZMQ_PUB);
	rc = zmq_bind(publisher, "tcp://*:5556");
	assert(rc == 0);

	rc = 0;
	while(TRUE) {

		asprintf(&str, "queue0 %d", rc);
		s_send(publisher, str);
		free(str);
		if (signaled)
			break;


		asprintf(&str, "queue1 %d", rc);
		s_send(publisher, str);
		free(str);
		if (signaled)
			break;

		sleep(1);
		rc++;
	}

	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}
