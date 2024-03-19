#include "../includes/malloc.h"

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(Zone *head, size_t size)
{
	for (Zone *zone_it = head; zone_it != NULL; zone_it = zone_it->next) {
		for (Block *block_it = zone_it->free; block_it != NULL;
		     block_it = block_it->next_free) {
			if (!block_it->in_use && size <= block_it->size) {
				if (block_it->zone != zone_it)
					block_it->zone = zone_it;
				return (block_it);
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
static void frag_block(Block *old_block, size_t size)
{
	bool empty = false;
	if (old_block->size < size - sizeof(Block)) {
		return;
	}

	Zone *zone = old_block->zone;
	Block *left = old_block->prev_free;
	Block *right = old_block->next_free;

	if (old_block->size - size <= 0) {
		empty = true;
		goto next;
	}
	Block *new_block = (Block *)((size_t)old_block + size);
	/* ft_printf("old_block: %p - ", old_block); */
	/* ft_printf("old_block->zone: %p - ", old_block->zone); */
	/* ft_printf("old_block->zone->free: %p\n", old_block->zone->free); */
	new_block->zone = old_block->zone;
	/* ft_printf("new_block: %p - ", new_block); */
	/* ft_printf("new_block->zone: %p - ", new_block->zone); */
	/* ft_printf("new_block->zone->free: %p\n", new_block->zone->free); */
	if (new_block == old_block->zone->free)
		return;

	new_block->size = old_block->size - (size - sizeof(Block));
	new_block->in_use = false;
	new_block->ptr = (void *)align_mem((size_t)new_block + sizeof(Block));
	new_block->zone = zone;

	new_block->prev = old_block;
	/* ft_printf("old_block: %p\n", old_block); */
	new_block->next = old_block->next;
	old_block->next = new_block;

	new_block->prev_used = NULL;
	new_block->next_used = NULL;

	new_block->prev_free = left;
	if (right != new_block)
		new_block->next_free = right;

	zone->free = new_block;

next:
	if (empty)
		zone->free = old_block->next_free;
	old_block->next_free = NULL;

	// newly in_use block metadata
	old_block->in_use = true;
	old_block->size = size - sizeof(Block);

	if (zone->used == NULL) {
		zone->used = old_block;
		return;
	}
	old_block->next_used = zone->used;
	zone->used->prev_used = old_block;
	zone->used = old_block;
}

void *ft_malloc(size_t size)
{
	// If mmap fails, the allocator won't be able to init correctly
	if (init_allocator() == -1) {
		ft_dprintf(2, "malloc: couldn't init allocator\n");
		return (NULL);
	}
	if (size == 0) {
		ft_dprintf(2, "malloc: can't malloc(0)\n");
		return (NULL);
	}

	// Find the zone we need to search
	block_type_t type = get_type(size);
	Zone *head = get_zone_head(type);

	// Find an available block in a zone of type "type"
	Block *available = find_block(head, size);
	if (available == NULL) {
		size_t full_size;
		if (type == LARGE)
			full_size = size + sizeof(Block);
		else
			full_size = get_zone_size(type);
		if (new_zones(type, full_size, 1) == -1)
			return (NULL);
		Zone *head = get_zone_head(type);
		available = find_block(head, size);
	}
	/* ft_printf("available: %p\n", available); */
	/* ft_printf("head->free: %p\n", head->free); */
	frag_block(available, size + sizeof(Block));
	return (available->ptr);
}
