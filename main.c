/* #include "includes/malloc.h" */
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

void *ft_malloc(size_t size);
void show_alloc_mem(void);

#define STRESS 0

int main(void)
{
#if STRESS
	show_alloc_mem();
	for (size_t i = 0; i < 100; ++i) {
		/* printf("%ld\n", i); */
		(void)ft_malloc(32);
	}
	show_alloc_mem();
#else
	show_alloc_mem();
	for (size_t i = 0; i < 10; ++i) {
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
