#include "includes/malloc.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 1000000

#define STRESS 0

int main(void)
{
#if STRESS
	// This is super fast
	/* printf("ft_malloc() with size %d\n", SIZE); */
	/* show_alloc_mem(); */
	/* for (size_t i = 0; i < SIZE; ++i) { */
	/* 	(void)ft_malloc(32); */
	/* 	/1* printf("[%ld] %p\n", i, ft_malloc(32)); *1/ */
	/* } */

	// This is super slow
	printf("ft_free(ft_malloc()) with size %d\n", SIZE);
	for (size_t i = 0; i < SIZE; ++i) {
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
	void *ptr = ft_realloc(tab[2], 2356);
	void *ptr2 = ft_malloc(2400);
	void *ptr3 = ft_realloc(tab[3], 20);
	void *ptr4 = ft_realloc(ptr3, 18);
	void *ptr5 = ft_realloc(ptr4, 25);
	/* for (int i = 7; i >= 0; --i) { */
	/* 	ft_free(tab[i]); */
	/* 	/1* show_alloc_mem(); *1/ */
	/* } */
	/* show_alloc_mem(); */
	/* ft_free(tab[3]); */
	/* show_alloc_mem(); */
	/* ft_free(tab[5]); */
	/* show_alloc_mem(); */
	/* ft_free(tab[0]); */
	/* show_alloc_mem(); */
	/* ft_free(tab[1]); */
	/* ft_free(tab[4]); */
	/* ft_free(tab[6]); */
	/* ft_free(tab[2]); */
	/* ft_free(tab[7]); */
	/* for (int i = 0; i < 8; ++i) { */
	/* 	ft_free(tab[i]); */
	/* 	/1* show_alloc_mem(); *1/ */
	/* } */
	/* show_alloc_mem(); */
	/* printf("\n"); */
	/* ft_free(ptr); */
	/* show_alloc_mem(); */
	/* printf("new malloc: %p\n", ft_malloc(32)); */
	/* show_alloc_mem(); */
	ft_free(ptr);
	ft_free(ptr2);
	ptr = ft_realloc(tab[4], 2356);
	show_alloc_mem();
#endif
	return (0);
}
