#include "../includes/malloc.h"

void *ft_realloc(void *ptr, size_t size)
{
	ft_printf("ptr: %p\n", ptr);
	if (ptr == NULL)
		return (NULL);
	Block *block = (Block *)((size_t)ptr - sizeof(Block));
	if (block->size >= size) {
		block->sub_size = size;
		return (ptr);
	}
	void *const new_ptr = ft_malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memmove(new_ptr, ptr, size);
	ft_free(ptr);
	return (new_ptr);
}
