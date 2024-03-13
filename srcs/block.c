#include "../includes/malloc.h"
#include <stdint.h>

static Block *find_block(block_type_t type)
{
	for (Zone *zone_it = zones; zone_it != NULL; zone_it = zone_it->next) {
		if (zone_it->type == type) {
			for (Block *block_it = zone_it->head; zone_it != NULL; zone_it = zone_it->next) {
				if (block_it->in_use == false)
					return (block_it);
			}
		}
	}
	return (NULL);
}

void *get_block(size_t size)
{
	block_type_t type;

	if (size <= TINY)
		type = TINY;	
	else if (size <= SMALL)
		type = SMALL;
	else
		type = LARGE;
	Block *available = find_block(type);
	available->size = size;
	available->in_use = true;
	return ((void*)((size_t)available + sizeof(Block)));
}
