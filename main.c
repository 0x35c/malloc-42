#include "includes/malloc.h"

int main(void)
{
	/* while (1) { */
	/* 	if (malloc(30000) == NULL) */
	/* 		return (EXIT_FAILURE); */
	/* } */
	for (size_t i = 0; i < 10; ++i) {
		(void)malloc(256);
	}
	show_alloc_mem();
	return (EXIT_SUCCESS);
}
