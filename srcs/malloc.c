#include "../includes/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(Zone *zone, size_t size)
{
	block_type_t type = get_type(size);
	/* printf("trying to find block in zone %p - zone->next %p\n", zone, */
	/*        zone->next); */

	for (Zone *zone_it = zone; zone_it != NULL; zone_it = zone_it->next) {
		for (Block *block_it = zone_it->free; block_it != NULL;
		     block_it = block_it->next_free) {
			/* printf("BLOCK [%p] -- searching block with " */
			/*        "block_it->size: %ld - " */
			/*        "block_it->size %% get_max_size(): %ld\n", */
			/*        block_it, block_it->size, */
			/*        block_it->size % get_max_size(type)); */
			if (!block_it->in_use &&
			    block_it->size % get_max_size(type) >= size) {
				/* printf("block found: %p\n", block_it->ptr);
				 */
				return (block_it);
			} else {
				if (!block_it->in_use) {
					/* printf("owno\n"); */
				}
			}
		}
	}
	return (NULL);
}

/* PARTIALLY DEPRECATED */
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
	static int count = 0;
	printf("[%d] frag block %p in zone %p\n", count, old_block, zone);
	if (old_block->size - size < sizeof(Block)) {
		printf("bozo %ld\n", old_block->size);
		return;
	}

	Block *left = old_block->prev_free;
	Block *right = old_block->next_free;
	Block *new_block = (Block *)((size_t)old_block + size);

	new_block->size = old_block->size - (size - sizeof(Block));
	/* new_block->size = size - sizeof(Block); */
	new_block->in_use = false;
	new_block->ptr =
	    (Block *)(((size_t)new_block + sizeof(Block) + MEM_ALIGN) &
	              ~(MEM_ALIGN));

	new_block->prev = old_block;
	new_block->next = old_block->next;
	old_block->next = new_block;

	new_block->prev_used = NULL;
	new_block->next_used = NULL;

	new_block->prev_free = left;
	new_block->next_free = right;

	zone->free = new_block;

	printf("[%d] FREE\n", count);
	int i = 0;
	for (Block *it = zone->free; it != NULL; it = it->next_free) {
		printf("free [%d] %p\n", i, it);
		i++;
	}

	old_block->next_free = NULL;
	if (zone->used == NULL) {
		zone->used = old_block;
		count++;
		return;
	}
	old_block->next_used = zone->used;
	zone->used = old_block;

	printf("[%d] USED\n", count);
	i = 0;
	for (Block *it = zone->used; it != NULL; it = it->next_used) {
		printf("used [%d] %p\n", i, it);
		i++;
	}
	count++;
}

void *ft_malloc(size_t size)
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
		size_t full_size;
		if (type == LARGE)
			full_size = size + sizeof(Block);
		else
			full_size = get_zone_size(type);
		if (new_zones(type, full_size, 1) == -1)
			return (NULL);
		available = find_block(zone, size);
	}
	available->in_use = true;
	available->size = size;
	frag_block(available, size + sizeof(Block), zone);
	return (available->ptr);
}
