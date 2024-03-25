#include "../includes/malloc.h"

pthread_mutex_t g_thread_safe = PTHREAD_MUTEX_INITIALIZER;

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(Zone *head, size_t size, Zone **current_zone)
{
	for (Zone *zone_it = head; zone_it != NULL; zone_it = zone_it->next) {
		for (Block *block_it = zone_it->free; block_it != NULL;
		     block_it = block_it->next_free) {
			assert(!block_it->in_use);
			if (size <= block_it->size) {
				assert(block_it->zone &&
				       block_it->zone == zone_it);
				*current_zone = zone_it;
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
static void frag_block(Zone *zone, Block *old_block, size_t size)
{
	Block *new_block = (Block *)align_mem((size_t)old_block + size);
	assert(!(new_block >=
	         (Block *)((size_t)zone + get_zone_size(zone->type))));
	new_block->size = old_block->size - size;
	new_block->sub_size = new_block->size;
	new_block->in_use = false;
	new_block->ptr = (void *)((size_t)new_block + sizeof(Block));
	new_block->zone = zone;

	new_block->prev = old_block;
	new_block->next = old_block->next;
	old_block->next = new_block;

	new_block->prev_used = NULL;
	new_block->next_used = NULL;

	new_block->prev_free = old_block->prev_free;
	new_block->next_free = old_block->next_free;

	if (zone->free == old_block)
		zone->free = new_block;

	old_block->next_free = NULL;
	old_block->prev_free = NULL;

	// newly in_use block metadata
	old_block->in_use = true;
	old_block->size = size - sizeof(Block);
	old_block->sub_size = old_block->size;

	if (zone->used == NULL) {
		zone->used = old_block;
		return;
	}
	old_block->prev_used = NULL;
	old_block->next_used = zone->used;
	zone->used->prev_used = old_block;
	zone->used = old_block;
}

static void save_block(Zone *head, Block *block, Zone *zone)
{
	zone->free = NULL;
	block->in_use = true;
	if (head->used) {
		head->used->prev_used = block;
		head->used->prev = block;
		block->next = head->used;
		block->next_used = head->used;
	}
	head->used = block;
}

void *ft_malloc(size_t size)
{
	pthread_mutex_lock(&g_thread_safe);
	void *ptr = NULL;
	// If mmap fails, the allocator won't be able to init correctly
	if (init_allocator() == -1) {
		ft_dprintf(2, "malloc: couldn't init allocator\n");
		goto end;
	}
	if (size == 0) {
		ft_dprintf(2, "malloc: can't malloc(0)\n");
		goto end;
	}

	// Find the zone we need to search
	block_type_t type = get_type(size);
	Zone *head = zones[type];
	Zone *zone = NULL;

	// Find an available block in a zone of type "type"
	Block *available = find_block(head, size, &zone);
	if (available == NULL) {
		size_t full_size;
		if (type == LARGE)
			full_size = size + sizeof(Block) + sizeof(Zone);
		else
			full_size = get_zone_size(type);
		if (new_zones(type, full_size, 1) == -1)
			goto end;
		head = zones[type];
		available = find_block(head, size, &zone);
	}
	assert(available != NULL);
	if (type == LARGE)
		save_block(head, available, zone);
	else
		frag_block(zone, available, size + sizeof(Block));
	ptr = available->ptr;
end:
	pthread_mutex_unlock(&g_thread_safe);
	return (ptr);
}
