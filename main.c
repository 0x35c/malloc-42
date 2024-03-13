#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "includes/malloc.h"

int main(void)
{
	char *str = malloc(13);
	if (str != NULL) {
		strcpy(str, "Hello world!");
		printf("%s\n", str);
	} else
		printf("Rip bozo\n");
	//show_alloc_mem();
	return (EXIT_SUCCESS);
}
