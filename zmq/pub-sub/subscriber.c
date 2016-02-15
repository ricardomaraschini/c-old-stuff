#include "common.h"

extern int	 signaled;

int
main(int argc, char *argv[])
{
	void		*context = NULL;
	void		*subscriber = NULL;
	char		*job = NULL;
	int		 rc = OK;

	if (argc == 1) {
		printf("you need to provide a queue\n");
		return 1;
	}

	signal(SIGTERM, sighandler);

	context = zmq_ctx_new();
	subscriber = zmq_socket(context, ZMQ_SUB);
	rc = zmq_connect(subscriber, "tcp://localhost:5556");
	assert(rc == 0);
	
	printf("listening on queue '%s'\n", argv[1]);
	rc = zmq_setsockopt(
	    subscriber,
	    ZMQ_SUBSCRIBE,
	    argv[1],
	    strlen(argv[1])
	);
	assert(rc == 0);

	while(TRUE) {
		if (signaled)
			break;

		if (s_recv(subscriber, &job) != OK)
			continue;

		printf("%s\n", job);
		free(job);
		job = NULL;
	}

	zmq_close(subscriber);
	zmq_ctx_destroy(context);

	return 0;
}
