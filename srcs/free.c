#include "../includes/malloc.h"

void free(void *ptr)
{
	if (ptr == NULL)
		return;
	Block *to_free = (Block *)(size_t)ptr - sizeof(Block);
	to_free->in_use = false;
}
