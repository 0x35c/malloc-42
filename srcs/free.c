#include "../includes/malloc.h"

static void remove_used(Block *to_free)
{
	Block *left = to_free->prev_used;
	Block *right = to_free->next_used;

	to_free->next_used = NULL;
	to_free->prev_used = NULL;

	if (!left && !right) {
		to_free->zone->used = NULL;
		return;
	}
	if (!left)
		to_free->zone->used = right;
	else
		left->next_used = right;
	if (right)
		right->prev_used = left;
}

/* If all the blocks of the zone have been freed,
 * we can unmap the zone and delete it from the list of zones
 */
static void unmap_zone(Zone *zone)
{
	block_type_t type = zone->type;
	Zone *left = zone->prev;
	Zone *right = zone->next;
	zone->prev = NULL;
	zone->next = NULL;

	if (!left && !right) {
		zones[type] = NULL;
		return;
	}
	if (!left)
		zones[type] = right;
	else
		left->next = right;
	if (right)
		right->prev = left;
	munmap(zone, get_zone_size(zone->type));
}

/* If the newly freed block is next to another previously
 * freed block, merge both of these and update the size
 */
static Block *merge_blocks(Block *left, Block *right)
{
	if (right->next)
		right->next->prev = left;
	if (right->next_free) {
		right->next_free->prev_free = left;
		left->next_free = right->next_free;
	}
	left->next = right->next;
	left->size += right->size + sizeof(Block);
	return (left);
}

// Simply add the new block to the list of available blocks
static void add_available(Block *available, Block *merged)
{
	Zone *zone = available->zone;
	if (merged != zone->free && available != zone->free)
		available->next_free = zone->free;
	zone->free->prev_free = available;
	zone->free = available;
	if (zone->free->next == NULL && zone->free->prev == NULL)
		unmap_zone(zone);
}

/*
 * ptr: pointer to free, if the pointer is invalid the free()
 * function will have an undefined behavior (most likely segfault)
 *
 * First, we remove the block from the list of in_use blocks
 * Then, we check if the block needs to be merged with another
 * neighboring block, if so we replace the previous block by the
 * newly merged block
 * Finally, we add the block to the list of available blocks
 */
void free(void *ptr)
{
	pthread_mutex_lock(&g_thread_safe);
	if (ptr == NULL)
		goto end;
	Block *to_free = (Block *)((size_t)ptr - sizeof(Block));
	Block *to_merge = NULL;
	to_free->in_use = false;
	remove_used(to_free);
	if (to_free->prev && !to_free->prev->in_use) {
		to_merge = to_free;
		to_free = merge_blocks(to_free->prev, to_free);
	}
	if (to_free->next && !to_free->next->in_use) {
		to_merge = to_free->next;
		to_free = merge_blocks(to_free, to_free->next);
	}
	add_available(to_free, to_merge);
end:
	pthread_mutex_unlock(&g_thread_safe);
}
