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

static Block *merge_blocks(Block *left, Block *right)
{
	if (right->next) {
		right->next->prev = left;
		right->next->prev_free = left;
	}
	left->next = right->next;
	if (left->next_free != left->zone->free->next_free)
		left->next_free = right->next_free;
	left->size += right->size;
	return (left);
}

static void add_available(Block *available)
{
	Zone *zone = available->zone;
	if (zone->free == NULL) {
		zone->free = available;
	} else if (available->size ==
	           get_zone_size(zone->type) - sizeof(Zone)) {
		available->next_free = NULL;
		available->next = NULL;
		zone->free->next_free = NULL;
		zone->free->next = NULL;
		zone->free = available;
	} else if (available != zone->free) {
		available->next_free = zone->free;
		zone->free->prev_free = available;
		zone->free = available;
	}
}

void ft_free(void *ptr)
{
	if (ptr == NULL)
		return;
	Block *to_free = (Block *)((size_t)ptr - sizeof(Block));
	to_free->in_use = false;
	remove_used(to_free);
	if (to_free->prev && !to_free->prev->in_use) {
		to_free = merge_blocks(to_free->prev, to_free);
	}
	if (to_free->next && !to_free->next->in_use) {
		to_free = merge_blocks(to_free, to_free->next);
	}
	add_available(to_free);
}
