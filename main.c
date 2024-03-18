/* #include "includes/malloc.h" */
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 10000000

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void show_alloc_mem(void);

#define STRESS 0

int main(void)
{
#if STRESS
	/* printf("ft_malloc with size %d\n", SIZE); */
	printf("ft_free(ft_malloc) with size %d\n", SIZE);
	for (size_t i = 0; i < SIZE; ++i) {
		/* (void)ft_malloc(32); */
		ft_free(ft_malloc(32));
	}
#else
	show_alloc_mem();
	/* void *ptr = ft_malloc(32); */
	void *tab[8];
	for (size_t i = 0; i < 8; ++i) {
		/* ft_free(ft_malloc(32)); */
		tab[i] = ft_malloc(32);
		/* printf("\n\n\n"); */
		/* show_alloc_mem(); */
		/* printf("[%ld] %p\n", i, ft_malloc(32)); */
	}
	show_alloc_mem();
	for (size_t i = 0; i < 8; ++i) {
		if (i == 4)
			show_alloc_mem();
		ft_free(tab[i]);
	}
	show_alloc_mem();
	/* printf("\n"); */
	/* ft_free(ptr); */
	/* show_alloc_mem(); */
	/* printf("new malloc: %p\n", ft_malloc(32)); */
	/* show_alloc_mem(); */
#endif
	return (0);
}
