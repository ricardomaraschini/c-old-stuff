#include "common.h"

int
main(void)
{
	void	*context = NULL;
	void	*receiver = NULL;
	void	*sender = NULL;
	char	*line = NULL;
	int	 i = 0;
	char	*m = NULL;
	int	 letter;
	int	 count[256] = { 0 };
	
	context = zmq_ctx_new();
	receiver = zmq_socket(context, ZMQ_PULL);
	set_socket_timeout(receiver);
	zmq_connect(receiver, VENTENDPOINT);

	sender = zmq_socket(context, ZMQ_PUSH);
	set_socket_timeout(sender);
	zmq_connect(sender, SINKENDPOINT);

	while(TRUE) {
		if (s_recv(receiver, &line) < 0) {
			perror("s_recv");
			continue;
		}

		for(i=0; i<strlen(line); i++) {
			letter = (int)line[i];
			if (!isalpha(letter))
				continue;
			count[letter]++;
		}

		for(i=0; i<strlen(line); i++) {

			letter = (int)line[i];
			if (!isalpha(letter))
				continue;

			asprintf(&m, "%d\t%d", letter, count[letter]);
			count[letter] = 0;

			if (s_send(sender, m) > 0) {
				free(m);
				continue;
			}

			perror("s_send");
			free(m);
		}
		free(line);
	}

	zmq_close(receiver);
	zmq_close(sender);
	zmq_ctx_destroy(context);

	return 0;
}
