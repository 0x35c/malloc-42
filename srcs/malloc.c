#include "../includes/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(Zone *zone, size_t size)
{
	for (Zone *zone_it = zone; zone_it != NULL; zone_it = zone_it->next) {
		for (Block *block_it = zone_it->free; block_it != NULL;
		     block_it = block_it->next) {
			if (block_it->in_use == false &&
			    block_it->size >= size) {
				return (block_it);
			}
		}
	}
	return (NULL);
}

/* DEPRECATED */
/* This will split the newly allocated block to use
 * the remaining bytes for a new block
 * This is our linked list of blocks
 * ... -> [5] -> [6] -> ...
 * After the allocation, this will become
 * ... -> [5] -> [new] -> [6] -> ...
 *
 * For an example of [5].size = 32 and requiring a malloc of 10
 * Let's say the metadata takes a size of 2:
 * ... -> [metadata][data][remaining size] -> [6]
 *            ^        ^         ^
 *            2       10        20
 *
 * So now our block [new] will become:
 * [5] -> [metadata][available data] -> [6]
 *             ^           ^
 *             2          18
 * We can see that it now has its own metadata and available
 * data and it points towards [6]
 */
static void frag_block(Block *old_block, size_t size, Zone *zone)
{
	if (old_block->size - size < sizeof(Block))
		return;

	Block *new_block = (Block *)((size_t)old_block + size);
	new_block->prev = old_block;
	new_block->next = old_block->next;
	new_block->next_used = NULL;
	new_block->next_free = old_block->next_free;
	new_block->size = old_block->size - size - sizeof(Block);
	new_block->in_use = false;
	new_block->ptr = (Block *)((size_t)new_block + sizeof(Block));

	// Set the previous block to point to the newly created block
	Block *prev = old_block->prev;
	if (prev == NULL)
		zone->free = new_block;
	else
		prev->next_free = new_block;

	/* int i = 0; */
	/* for (Block *it = zone->free; it != NULL; it = it->next_free) { */
	/* 	printf("[%d] %p\n", i, it); */
	/* 	i++; */
	/* } */

	old_block->next_free = NULL;
	old_block->next = new_block;
	if (zone->used == NULL) {
		zone->used = old_block;
		return;
	}
	old_block->next_used = zone->used;
	zone->used = old_block;
}

void *malloc(size_t size)
{
	// If mmap fails, the allocator won't be able to init correctly
	if (init_allocator() == -1) {
		ft_dprintf(2, "couldn't init allocator\n");
		return (NULL);
	}
	if (size == 0) {
		ft_dprintf(2, "can't malloc(0)\n");
		return (NULL);
	}

	// Find the zone we need to search
	block_type_t type = get_type(size);
	Zone *zone = get_zone(type);

	// Find an available block in a zone of type "type"
	Block *available = find_block(zone, size);
	if (available == NULL) {
		if (new_zone(type, get_max_size(type)) == -1)
			return (NULL);
		available = find_block(zone, size);
	}
	frag_block(available, size + sizeof(Block), zone);
	available->in_use = true;
	available->size = size;
	void *ptr = (void *)((size_t)available + sizeof(Block));
	return (ptr);
}
