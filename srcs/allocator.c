#include "../includes/malloc.h"
#include <unistd.h>

Zones *zones;

/* static void add_blocks(Zone *zone, size_t zone_size) */
/* { */
/* 	ft_printf("add_blocks()\n"); */
/* 	// Total size of the zone, to make sure we do not assign an address */
/* 	// after the actual zone we have */
/* 	const size_t block_size = get_max_size(zone->type) + sizeof(Block); */
/* 	Block *iter = (Block *)((size_t)zone + sizeof(Zone)); */
/* 	zone->free = iter; */
/* 	Block *prev = NULL; */
/* 	do { */
/* 		// Metadata */
/* 		iter->ptr = */
/* 		    (Block *)(((size_t)iter + sizeof(Block) + MEM_ALIGN) & */
/* 		              ~(MEM_ALIGN)); */
/* 		iter->size = block_size - sizeof(Block); */
/* 		iter->in_use = false; */

/* 		// Base list (aligned blocks in memory) */
/* 		iter->prev = prev; */
/* 		iter->next = (Block *)((size_t)iter + block_size); */

/* 		// Available blocks list */
/* 		iter->prev_free = prev; */
/* 		iter->next_free = iter->next; */

/* 		// In use blocks list */
/* 		iter->prev_used = NULL; */
/* 		iter->next_used = NULL; */

/* 		prev = iter; */
/* 		iter = iter->next; */
/* 	} while ((size_t)iter + block_size <= (size_t)zone + zone_size); */
/* 	iter->next = NULL; */
/* } */

static void append_list(Zone *head, block_type_t type)
{
	Zone **begin;
	if (type == TINY)
		begin = &zones->tiny;
	else if (type == SMALL)
		begin = &zones->small;
	else
		begin = &zones->large;

	// If the list is empty, put new_zone at the beginning
	if (*begin == NULL) {
		*begin = head;
		return;
	}

	// Else, we put the zone at the end
	Zone *it = *begin;
	while (it->next)
		it = it->next;
	it->next = head;
}

static void new_block(Zone *zone, size_t zone_size)
{
	Block *new_block = (Block *)((size_t)zone + sizeof(Zone));

	// Metadata
	new_block->in_use = false;
	new_block->size = zone_size - sizeof(Zone);
	new_block->ptr =
	    (Block *)(((size_t)new_block + sizeof(Block) + MEM_ALIGN) &
	              ~(MEM_ALIGN));

	// Init future linked lists
	new_block->next = NULL;
	new_block->prev_free = NULL;
	new_block->next_free = NULL;
	new_block->prev_used = NULL;
	new_block->next_used = NULL;

	zone->free = new_block;
}

int new_zones(block_type_t type, size_t zone_size, size_t nb_zones)
{
	struct rlimit limit;
	if (getrlimit(RLIMIT_AS, &limit) == -1) {
		ft_dprintf(2, "error: syscall getrlimit failed\n");
		return (-1);
	}
	if (zone_size * nb_zones >= limit.rlim_max) {
		ft_dprintf(2, "error: no more memory available\n");
		return (-1);
	}

	void *heap = mmap(NULL, zone_size * nb_zones, PROT_READ | PROT_WRITE,
	                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (heap == NULL) {
		ft_dprintf(2, "error: syscall mmap failed\n");
		return (-1);
	}

	// Little trick to make it have a good value when doing first loop
	Zone *new_zone = (Zone *)((size_t)heap - zone_size);
	Zone *prev = NULL;
	for (size_t count = 0; count < nb_zones; ++count) {
		new_zone = (Zone *)((size_t)new_zone + zone_size);
		if (count > 0 && count + 1 < nb_zones)
			prev->next = new_zone;
		prev = new_zone;
		new_zone->type = type;
		new_zone->used = NULL;
		new_zone->next = NULL;
		new_block(new_zone, zone_size);
	}

	append_list(new_zone, type);

	printf("zones->tiny->free: %p\n", zones->tiny->free);

	return (0);
}

int init_allocator(void)
{
	ft_printf("init_allocator()\n");
	static bool initialized = false;
	if (initialized)
		return (0);
	initialized = true;

	int err;

	// We take 1 page to store the linked list for all the zones
	zones = (Zones *)mmap(NULL, sizeof(Zones), PROT_READ | PROT_WRITE,
	                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	zones->tiny = NULL;
	zones->small = NULL;
	zones->large = NULL;

	err = new_zones(TINY, PAGES_TINY * getpagesize(), INIT_ZONES);
	if (err < 0)
		return (err);
	err = new_zones(SMALL, PAGES_SMALL * getpagesize(), INIT_ZONES);
	if (err < 0)
		return (err);
	return (0);
}
