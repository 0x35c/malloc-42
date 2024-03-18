/* #include "includes/malloc.h" */
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

void *ft_malloc(size_t size);
void show_alloc_mem(void);

#define STRESS 1

int main(void)
{
#if STRESS
	/* show_alloc_mem(); */
	for (size_t i = 0; i < 10000000; ++i) {
		/* printf("%ld\n", i); */
		(void)ft_malloc(32);
	}
	/* show_alloc_mem(); */
#else
	show_alloc_mem();
	/* (void)ft_malloc(32); */
	for (size_t i = 0; i < 13; ++i) {
		(void)ft_malloc(32);
		/* printf("\n\n\n"); */
		/* show_alloc_mem(); */
		/* printf("[%ld] %p\n", i, ft_malloc(32)); */
	}
	printf("\n");
	show_alloc_mem();
#endif
	return (0);
}
