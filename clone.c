#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err_exit(msg) do { \
	perror(msg); \
	exit(EXIT_FAILURE); \
} while (0)

#define STACK_SIZE (1024 * 1024) /* Stack size for cloned child */

/* Start function for cloned child */
static int child_func(void *arg)
{
	struct utsname uts;

	/* Change hostname in UTS namespace of child */
	if (sethostname(arg, strlen(arg)) == -1)
		err_exit("sethostname");

	/* Retrieve and display hostname */
	if (uname(&uts) == -1)
		err_exit("uname");
	printf("uts.nodename in child: %s\n", uts.nodename);

	/* Keep the namespace open for a while, by sleeping.
	   This allows some experimentation--for example, another
	   process might join the namespace. */
	sleep(200);

	return 0; /* Child terminates now */
}

int main(int argc, char *argv[])
{
	char *stack; /* Start of stack buffer */
	char *stack_top; /* End of stack buffer */
	pid_t pid;
	struct utsname uts;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	/* Allocate stack for child */
	stack = malloc(STACK_SIZE);
	if (stack == NULL)
		err_exit("malloc");
	stack_top = stack + STACK_SIZE; /* Assume stack grows downward */

	/* Create child that has its own UTS namespace;
	   child commences execution in child_func() */
	pid = clone(child_func, stack_top, CLONE_NEWUTS | SIGCHLD, argv[1]);
	if (pid == -1)
		err_exit("clone");
	printf("clone() returned %ld\n", (long) pid);

	/* Parent falls through to here */
	sleep(1); /* Give child time to change its hostname */

	/* Display hostname in parent's UTS namespace. This will be
	   different from hostname in child's UTS namespace. */
	if (uname(&uts) == -1)
		err_exit("uname");
	printf("uts.nodename in parent: %s\n", uts.nodename);

	if (waitpid(pid, NULL, 0) == -1) /* Wait for child */
		err_exit("waitpid");
	printf("child has terminated\n");
	free(stack);

	exit(EXIT_SUCCESS);
}

