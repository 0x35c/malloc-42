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
	left->zone->nb_blocks--;
	ft_printf("merging left: %p and right: %p\n", left, right);
	if (right->next) {
		right->next->prev = left;
		right->next->prev_free = left;
	}
	left->next = right->next;
	/* ft_printf("left->next_free: %p - left->zone->free: %p\n", */
	/*           left->next_free, left->zone->free); */
	if (left->next_free != left->zone->free->next_free)
		left->next_free = right->next_free;
	left->size += right->size;
	return (left);
}

static void add_available(Block *available)
{
	Zone *zone = available->zone;
	if (zone->nb_blocks <= 1) {
		zone->free = available;
		return;
	}
	zone->nb_blocks++;
	/* Block *head_free = zone->free; */
	/* available->prev_free = NULL; */
	/* available->next_free = NULL; */

	/* ft_printf("[0] zone: %p - zone->free: %p - zone->free->next_free:
	 * %p\n", */
	/*           zone, zone->free, zone->free->next_free); */
	/* if (zone->free == NULL) { */
	/* 	zone->free = available; */
	/* 	return; */
	/* } */
	/* ft_printf("[1] zone: %p - zone->free: %p - zone->free->next_free: %p
	 * - " */
	/*           "available->next_free: %p\n", */
	/*           zone, zone->free, zone->free->next_free, */
	/*           available->next_free); */
	/* Block *next = head_free; */
	/* ft_printf( */
	/*     "[1.1] zone: %p - zone->free: %p - zone->free->next_free: %p - "
	 */
	/*     "available->next_free: %p\n", */
	/*     zone, zone->free, zone->free->next_free, available->next_free);
	 */
	/* head_free->prev_free = available; */
	/* zone->free = available; */
	/* /1* zone->free->next_free = next; *1/ */
	/* ft_printf("[2] zone: %p - zone->free: %p - zone->free->next_free: %p
	 * " */
	/*           "with next: %p\n", */
	/*           zone, zone->free, zone->free->next_free, next); */

	printf("[42] available: %p - zone->free: %p\n", available, zone->free);
	if (available != zone->free)
		available->next_free = zone->free;
	else
		return;
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
	Zone *zone = to_free->zone;
	if (to_free->prev && !to_free->prev->in_use) {
		/* new_block = to_free->prev; */
		ft_printf("[69] zone->free: %p - zone->free->next_free: %p\n",
		          zone->free, zone->free->next_free);
		ft_printf("to merge: %p with prev block: %p\n", to_free,
		          to_free->prev);
		to_free = merge_blocks(to_free->prev, to_free);
		ft_printf("[69.1] zone->free: %p - zone->free->next_free: %p\n",
		          zone->free, zone->free->next_free);
		/* ft_ft_printf("to_free->prev now: %p with size: %ld\n", */
		/*           to_free->prev, to_free->prev->size); */
	}
	if (to_free->next && !to_free->next->in_use) {
		ft_printf("to merge with next block\n");
		to_free = merge_blocks(to_free, to_free->next);
	}
	ft_printf(
	    "[69.2] zone: %p - zone->free: %p - zone->free->next_free: %p\n",
	    zone, zone->free, zone->free->next_free);
	add_available(to_free);
	ft_printf("[69.3] zone->free: %p - zone->free->next_free: %p\n",
	          zone->free, zone->free->next_free);
}
