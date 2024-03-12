#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "includes/malloc.h"

int main(void)
{
	(void)malloc(10);
	show_alloc_mem();
	return (EXIT_SUCCESS);
}
