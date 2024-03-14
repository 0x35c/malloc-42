#include "includes/malloc.h"
#include "libs/libft/libs/ft_printf/includes/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
	/* (void)malloc(0); */
	/* show_alloc_mem(); */
	for (size_t i = 0; i < 100000; ++i) {
		(void)malloc(256);
	}
	/* show_alloc_mem(); */
	return (EXIT_SUCCESS);
}
