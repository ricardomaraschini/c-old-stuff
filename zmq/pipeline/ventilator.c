#include "common.h"

int
main(int argc, char *argv[])
{
	void	*context = NULL;
	void	*sender = NULL;
	FILE	*fp = NULL;
	char	 line[1024];

	if (argc <= 1) {
		printf("provide a file\n");
		return ERROR;
	}

	if (access(argv[1], F_OK) == -1 ) {
		printf("%s file does not exist\n", argv[1]);
		return ERROR;
	}
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("unable to open %s\n", argv[1]);
		return ERROR;
	}

	context = zmq_ctx_new();
	sender = zmq_socket(context, ZMQ_PUSH);
	set_socket_timeout(sender);
	zmq_bind(sender, VENTBIND);

	while(fgets(line, sizeof(line), fp)) {
		if (line[0] == '#')
			continue;

		if (s_send(sender, line) < 0)
			perror("s_send");
	}

	fclose(fp);
	zmq_close(sender);
	zmq_ctx_destroy(context);
	return OK;
}
