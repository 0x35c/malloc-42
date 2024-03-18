#include "../includes/malloc.h"

static void remove_used(Block *to_free)
{
	Block *used_head = to_free->zone->used;
	Block *left = to_free->prev_used;
	Block *right = to_free->next_used;

	to_free->next_used = NULL;
	to_free->prev_used = NULL;

	if (!left && !right) {
		to_free->zone->used = NULL;
		return;
	}
	if (left)
		left->next_used = right;
	else
		used_head = right;
	if (right)
		right->prev_used = left;
}

static void merge_blocks(Block *left, Block *right)
{
	if (right->next) {
		right->next->prev = left;
		right->next->prev_free = left;
	}
	left->next = right->next;
	left->next_free = right->next_free;
	left->size += right->size;
}

static void add_available(Block *available)
{
	Zone *zone = available->zone;
	available->prev_free = NULL;
	available->next_free = NULL;
	/* Block *new_block = available; */
	if (available->prev && !available->prev->in_use) {
		/* new_block = available->prev; */
		printf("to merge: %p with prev block: %p\n", available,
		       available->prev);
		merge_blocks(available->prev, available);
		printf("available->prev now: %p with size: %ld\n",
		       available->prev, available->prev->size);
	} else if (available->next && !available->next->in_use) {
		printf("to merge with next block\n");
		/* merge_blocks(available, available->next); */
	}

	available->next_free = zone->free;
	/* if (available->next == free_head) */
	/* 	merge_blocks(available, available->next); */
	/* else if (available->prev == available) { */
	/* } */
	if (zone->free == NULL) {
		zone->free = available;
		return;
	}
	zone->free->prev_free = available;
	zone->free = available;
}

void ft_free(void *ptr)
{
	if (ptr == NULL)
		return;
	Block *to_free = (Block *)((size_t)ptr - sizeof(Block));
	to_free->in_use = false;
	remove_used(to_free);
	add_available(to_free);
}
