#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int sck;

	if ((sck = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Error: socket(), %s, exiting\n", strerror(errno));
		return -1;
	}
	if (listen(sck, 10)) {
		printf("Error: listen(), %s, exiting\n", strerror(errno));
		goto Exit;
	}
	printf("Parent listening on socket %d\n", sck);

Exit:
	close(sck);
	return 0;
}
