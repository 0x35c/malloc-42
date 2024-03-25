#include "../includes/malloc.h"

void *ft_realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&g_thread_safe);
	void *new_ptr = NULL;
	if (ptr == NULL)
		goto end;
	Block *block = (Block *)((size_t)ptr - sizeof(Block));
	if (block->size >= size) {
		block->sub_size = size;
		pthread_mutex_unlock(&g_thread_safe);
		return (ptr);
	}
	pthread_mutex_unlock(&g_thread_safe);
	new_ptr = ft_malloc(size);
	pthread_mutex_lock(&g_thread_safe);
	if (new_ptr == NULL)
		goto end;
	ft_memmove(new_ptr, ptr, size);
	pthread_mutex_unlock(&g_thread_safe);
	ft_free(ptr);
	pthread_mutex_lock(&g_thread_safe);
end:
	pthread_mutex_unlock(&g_thread_safe);
	return (new_ptr);
}
