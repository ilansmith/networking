#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static int do_parent(void)
{
	int sck;

	/* parent */
	if ((sck = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Error (parent): socket(), %s, exiting\n",
			strerror(errno));
		return -1;
	}

	if (listen(sck, 10)) {
		printf("Error (parent): listen(), %s, exiting\n",
			strerror(errno));
		close(sck);
		return -1;
	}
	printf("Parent opened and listening on socket %d\n", sck);

	return sck;
}

static int do_child(void)
{
	int sck;

	if ((sck = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Error (child): socket(), %s, exiting\n",
				strerror(errno));
		return -1;
	}
	printf("Child opened socket %d\n", sck);

	return sck;
}

int main(int argc, char **argv)
{
	int sck_parent = -1, sck_child = -1, is_child = 0;

	if (!is_child && (sck_parent = do_parent()) == -1)
		return -1;

	if (!(fork())) {
		is_child = 1;

		if (is_child && (sck_child = do_child()) == -1)
			return -1;
	}

	if (sck_child != -1) {
		printf("Child closing socket %d\n", sck_child);
		close(sck_child);
	}

	if (sck_parent != -1) {
		printf("%s closing socket %d\n", is_child ? "Child" : "Parent",
			sck_parent);
		close(sck_parent);
	}
	return 0;
}

