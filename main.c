#include "includes/malloc.h"

int main(void)
{
	for (size_t i = 0; i < 100000; ++i) {
		(void)malloc(256);
	}
	return (EXIT_SUCCESS);
}
