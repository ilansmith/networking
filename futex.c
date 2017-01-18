#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/syscall.h>
#include<linux/futex.h>
#include<linux/unistd.h>
#include "futex.h"

#define ARRAY_SZ(arr) (sizeof(arr) / sizeof(arr[0]))

struct futex_routines {
	pthread_t p;
	void *(*routine)(void *);
};

static int m1;
static int m2;
 
/* example 1 */

static void *threadfn1a(void *p)
{
    printf("%s before lock\n", __FUNCTION__);
    futex(&m1, FUTEX_WAIT, m1, NULL, NULL, 0);
    printf("%s after lock\n", __FUNCTION__);
    sleep(2);
 
    return NULL;
}
 
static void *threadfn1b(void *p)
{
    sleep(2);
    printf("%s before lock\n", __FUNCTION__);
    futex(&m1, FUTEX_WAKE, 1, NULL, NULL, 0);
    printf("%s after lock\n", __FUNCTION__);
    return NULL;
}
 
static void example1(void)
{
	int i;
	struct futex_routines funcs[] = {
		{ .routine = threadfn1a },
		{ .routine = threadfn1b },
	};

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_create(&funcs[i].p, NULL, funcs[i].routine, NULL);

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_join(funcs[i].p, NULL);
}

/* example 2 */

static void *threadfn2a(void *p)
{
    printf("%s before lock\n", __FUNCTION__);
    futex(&m1, FUTEX_WAIT, m1, NULL, NULL, 0);
    printf("%s after lock\n", __FUNCTION__);
    sleep(2);
 
    return NULL;
}
 
static void *threadfn2b(void *p)
{
    sleep(2);
    printf("%s before lock\n", __FUNCTION__);
    futex(&m1, FUTEX_WAKE, 1, NULL, NULL, 0);
    printf("%s after lock\n", __FUNCTION__);
    return NULL;
}
 
static void example2(void)
{
	int i;
	struct futex_routines funcs[] = {
		{ .routine = threadfn2a },
		{ .routine = threadfn2b },
	};

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_create(&funcs[i].p, NULL, funcs[i].routine, NULL);

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_join(funcs[i].p, NULL);
}

/* example 3 */

static void *threadfn3a(void *p)
{
	printf("%s before lock\n", __FUNCTION__);
	futex(&m1, FUTEX_WAIT, m1, NULL, NULL, 0);
	printf("%s after lock\n", __FUNCTION__);
	return NULL;
}

static void *threadfn3b(void *p)
{
	printf("%s before lock\n", __FUNCTION__);
	futex(&m2, FUTEX_WAIT, m2, NULL, NULL, 0);
	printf("%s after lock\n", __FUNCTION__);
	return NULL;
}

static void *threadfn3c(void *p)
{
	sleep(2);
	printf("%s before lock\n", __FUNCTION__);
	futex(&m1, FUTEX_WAKE_OP, 1, NULL, &m2, FUTEX_OP(FUTEX_OP_ADD, 2,
		FUTEX_OP_CMP_EQ, 1));
	printf("%s after lock\n", __FUNCTION__);
	printf("%s m1=%d m2=%d\n", __FUNCTION__, m1, m2);
	return NULL;
}

static void example3(void)
{
	int i;
	struct futex_routines funcs[] = {
		{ .routine = threadfn3a },
		{ .routine = threadfn3b },
		{ .routine = threadfn3c },
	};

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_create(&funcs[i].p, NULL, funcs[i].routine, NULL);

	for (i = 0; i < ARRAY_SZ(funcs); i++)
		pthread_join(funcs[i].p, NULL);
}

int main(int argc, char **argv)
{
	int i;
	void (*examples[])(void) = {
		example1,
		example2,
		example3
	};

	for (i = 0; i < ARRAY_SZ(examples); i++) {
		m1 = 1;
		m2 = 1;

		printf("start example%d\n", i + 1);
		examples[i]();
		printf("end example%d\n\n", i + 1);
	}
	return 0;
}

