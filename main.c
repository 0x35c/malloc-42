#include "includes/malloc.h"

#define STRESS 0

int main(void)
{
#if STRESS
	for (size_t i = 0; i < 10000000; ++i) {
		(void)malloc(256);
	}
#else
	/* show_alloc_mem(); */
	for (size_t i = 0; i < 10; ++i) {
		printf("[%ld] %p\n", i, malloc(32));
	}
	/* show_alloc_mem(); */
#endif
	return (EXIT_SUCCESS);
}
