#include "includes/malloc.h"

int main(void)
{
	/* while (1) { */
	/* 	if (malloc(30000) == NULL) */
	/* 		return (EXIT_FAILURE); */
	/* } */
	for (size_t i = 0; i < 100000; ++i) {
		(void)malloc(256);
	}
	return (EXIT_SUCCESS);
}
