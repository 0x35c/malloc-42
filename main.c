#include "includes/malloc.h"

int main(void)
{
	for (size_t i = 0; i < 1000000; ++i) {
		(void)malloc(256);
	}
	/* show_alloc_mem(); */
	/* for (size_t i = 0; i < 100; ++i) { */
	/* 	printf("[%ld] %p\n", i, malloc(256)); */
	/* } */
	/* show_alloc_mem(); */
	return (EXIT_SUCCESS);
}
