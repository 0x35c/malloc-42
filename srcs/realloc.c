#include "malloc.h"

// Prototype for free and malloc
void free(void *ptr);
void *malloc(size_t size);

/*
 * ptr: block to resize (undefined behavior if invalid)
 * size: size needed by the user to get reallocated
 *
 * If we have a size <= to the previous size, we don't have
 * to do anything, we just change sub_size for info purposes
 * and return the same pointer
 * Else, we allocate a new block and copy the content of
 * the previous block in the new one and free the old block
 *
 * ptr: returns the aligned pointer of the reallocated block
 */
void *realloc(void *ptr, size_t size)
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
	new_ptr = malloc(size);
	pthread_mutex_lock(&g_thread_safe);
	if (new_ptr == NULL)
		goto end;
	ft_memmove(new_ptr, ptr, block->size);
	pthread_mutex_unlock(&g_thread_safe);
	free(ptr);
	pthread_mutex_lock(&g_thread_safe);
end:
	pthread_mutex_unlock(&g_thread_safe);
	return (new_ptr);
}
