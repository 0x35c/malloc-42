#include "includes/malloc.h"
#include <stdio.h>
#include <string.h>

#define SIZE 1000000

#define STRESS 0

int main(void)
{
#if STRESS
	/* printf("malloc() with %d iterations\n", SIZE); */
	/* for (size_t i = 0; i < SIZE; ++i) */
	/* 	(void)malloc(32); */

	printf("free(malloc()) with %d iterations\n", SIZE);
	for (size_t i = 0; i < SIZE; ++i)
		free(malloc(32));
#else
	char *tab[8];
	for (size_t i = 0; i < 8; ++i) {
		tab[i] = malloc(32);
		/* printf("tab[%ld]: %p\n", i, tab[i]); */
		/* printf("\n\n\n"); */
		/* show_alloc_mem(); */
	}
	strcpy(tab[2], "test\n");
	void *ptr = realloc(tab[2], 2356);
	/* printf("tab[2]: %s\n", tab[2]); */
	void *ptr2 = malloc(2400);
	/* show_alloc_mem(); */
	strcpy(tab[3], "test more\n");
	void *ptr3 = realloc(tab[3], 20);
	void *ptr4 = realloc(ptr3, 18);
	void *ptr5 = realloc(ptr4, 25);
	/* printf("ptr5: %s\n", (char *)ptr5); */
	/* for (int i = 7; i >= 0; --i) { */
	/* 	free(tab[i]); */
	/* 	/1* show_alloc_mem(); *1/ */
	/* } */
	/* show_alloc_mem(); */
	/* free(tab[3]); */
	/* show_alloc_mem(); */
	/* free(tab[5]); */
	/* show_alloc_mem(); */
	/* free(tab[0]); */
	/* show_alloc_mem(); */
	/* free(tab[1]); */
	/* free(tab[4]); */
	/* free(tab[6]); */
	/* free(tab[2]); */
	/* free(tab[7]); */
	/* for (int i = 0; i < 8; ++i) { */
	/* 	free(tab[i]); */
	/* 	/1* show_alloc_mem(); *1/ */
	/* } */
	/* show_alloc_mem(); */
	/* printf("\n"); */
	/* free(ptr); */
	/* show_alloc_mem(); */
	/* printf("new malloc: %p\n", malloc(32)); */
	/* show_alloc_mem(); */
	/* free(ptr); */
	/* free(ptr2); */
	ptr = realloc(tab[4], 2356);
	void *small = malloc(2048);
	show_alloc_mem();
#endif
	return (0);
}
