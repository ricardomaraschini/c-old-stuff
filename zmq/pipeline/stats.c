#include "common.h"


int
main(int argc, char *argv[])
{
	stmsg	 msg;
	int	 msglen;
	int	 signaled = 0;
	int	 msgid = 0;
	int	 ret = 0;
	int	 i = 0;
	int	 c = 0;
	int	 cleanstats = FALSE;
	PROCTAB	*proc = NULL;
	proc_t	 procinfo;

	while((c = getopt(argc, argv, "X")) != -1) {
		if (c == 'X')
			cleanstats = TRUE;
	}

	proc = openproc(PROC_FILLMEM|PROC_FILLSTAT|PROC_FILLSTATUS);
	memset(&procinfo, 0, sizeof(procinfo));
	while(readproc(proc, &procinfo) != NULL) {
		if (strcmp(procinfo.cmd, "sink") != 0)
			continue;

		if (cleanstats)
			kill(procinfo.tid, SIGUSR2);
		else
			kill(procinfo.tid, SIGUSR1);

		signaled = 1;
		break;
	}
	closeproc(proc);

	if (signaled == 0) {
		printf("unable to locate sink process\n");
		return ERROR;
	}

	if (cleanstats) {
		printf("clean signal sent\n");
		return OK;
	}


	msgid = msgget(MQUEUEKEY, 0666|IPC_CREAT);
	if (msgid == -1) {
		printf("unable to get message queue\n");
		return ERROR;
	}

	msglen = sizeof(msg) - sizeof(long);
	ret = msgrcv(msgid, (void *)&msg, msglen, MSGSTATUS, 0);
	if (ret == -1) {
		printf("failed to receive messages\n");
		msgctl(msgid, IPC_RMID, 0);
		return ERROR;
	}

	for(i=0; i<256; i++) {
		if (msg.count[i] == 0)
			continue;
		printf("%c\t%ld\n", (char)i, msg.count[i]);
	}

	msgctl(msgid, IPC_RMID, 0);
	return OK;
}
