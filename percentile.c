#include <stdlib.h>
#include <stdio.h>

#define MAGNETA "\e[2;35m"
#define RED "\e[0;31m"
#define ENDCOLOR "\e[0m"

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

struct object {
	double val;
};

static int obj_compare(const void *obj1, const void *obj2)
{
	struct object *o1 = (struct object*)obj1;
	struct object *o2 = (struct object*)obj2;

	return o1->val - o2->val;
}

static void object_init(struct object *obj)
{
	obj->val = rand() / 11;
}

static void objects_print(struct object *objs, int size)
{
	int i;

	for (i = 0; i < size; i++)
		printf("%-.3d. %f\n", i + 1, objs[i].val);
}

void print_percentiles(struct object *objs, int size)
{
	qsort(objs, size, sizeof(struct object), obj_compare);

	double percentile[] = {
		0.99999, 0.9999, 0.999, 0.99, 0.90, 0.75, 0.50, 0.25
	};

	printf(MAGNETA "Total %lu observations" ENDCOLOR "; each percentile "
		"contains %.2lf observations\n", (long unsigned)size,
		(double)size/100);

	printf("---> <MAX> observation = %8.3lf\n", objs[size-1].val);
	for (int i = 0; i < ARRAY_SIZE(percentile); i++) {
		int index = (int)(0.5 + percentile[i]*size) - 1;

		if (index >= 0) {
			printf("---> percentile %6.3lf = %8.3lf\n",
				percentile[i] * 100, objs[index].val);
		}
	}
	printf("---> <MIN> observation = %8.3lf\n", objs[0].val);
}

static int test(int size)
{
	struct object *objs;
	int i;

	srand(0);

	objs = malloc(size * sizeof (struct object));
	if (!objs)
		return -1;

	for (i = 0; i < size; i++)
		object_init(&objs[i]);

	printf("random:\n");
	objects_print(objs, size);

	print_percentiles(objs, size);
	free(objs);
	return 0;
}

int main(int argc, char **argv)
{
	test(100);
}

