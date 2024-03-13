#include "../includes/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(block_type_t type)
{
	for (Zone *zone_it = zones; zone_it != NULL; zone_it = zone_it->next) {
		if (zone_it->type == type) {
			for (Block *block_it = zone_it->head; block_it != NULL; block_it = block_it->next) {
				if (block_it->in_use == false)
					return (block_it);
			}
		}
	}
	return (NULL);
}

void *malloc(size_t size)
{
	// If mmap fails, the allocator won't be able to init correctly
	if (init_allocator() == -1) {
		ft_dprintf(2, "couldn't init allocator\n");
		return (NULL);
	}

	block_type_t type;
	if (size <= (size_t)PAGES_TINY * getpagesize())
		type = TINY;	
	else if (size <= (size_t)PAGES_SMALL * getpagesize())
		type = SMALL;
	else
		type = LARGE;

	Block *available = find_block(type);
	if (available == NULL)
		return (NULL); // We need more zones, TODO
	available->size = size;
	available->in_use = true;
	void *ptr = (void*)((size_t)available + sizeof(Block));
	return (ptr);
}
