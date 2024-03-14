#include "../includes/malloc.h"

block_type_t get_type(size_t size)
{
	if (size <= (size_t)PAGES_TINY * getpagesize() - sizeof(Block))
		return (TINY);
	if (size <= (size_t)PAGES_SMALL * getpagesize() - sizeof(Block))
		return (SMALL);
	return (LARGE);
}

Zone *get_zone(block_type_t type)
{
	if (type == TINY)
		return (zones->tiny);
	if (type == SMALL)
		return (zones->small);
	return (zones->large);
}

size_t get_max_size(block_type_t type)
{
	if (type == TINY)
		return (PAGES_TINY * getpagesize());
	if (type == SMALL)
		return (PAGES_SMALL * getpagesize());
	return (0);
}

void add_blocks(Zone *zone, size_t nb_blocks, size_t block_size)
{
	const size_t zone_size = (size_t)zone + nb_blocks * block_size;
	zone->free = (Block *)((size_t)zone + sizeof(Zone));
	Block *iter = zone->free;
	Block *prev = NULL;
	do {
		iter->ptr = (Block *)((size_t)iter + sizeof(Block));
		iter->size = block_size - sizeof(Block);
		iter->in_use = false;
		iter->prev = prev;
		iter->next = (Block *)((size_t)iter + block_size);
		iter->next_used = NULL;
		iter->next_free = iter->next;
		prev = iter;
		iter = iter->next;
	} while ((size_t)iter + block_size <= zone_size);
	prev = (Block *)((size_t)iter - block_size);
	prev->next = NULL;
	iter->next = NULL;
}

static void append_zone(Zone *new_zone, block_type_t type)
{
	/* static int count = 0; */
	Zone *it;
	// Add the new zone to the end of the Zones
	if (type == TINY) {
		if (zones->tiny == NULL) {
			zones->tiny = new_zone;
			return;
		}
		it = zones->tiny;
	} else if (type == SMALL) {
		if (zones->small == NULL) {
			zones->small = new_zone;
			return;
		}
		it = zones->small;
	} else {
		if (zones->large == NULL) {
			zones->large = new_zone;
			return;
		}
		it = zones->large;
	}
	while (it) {
		/* if (type == TINY) { */
		/* 	printf("[%d.0] zone tiny inside loop %p\n", count, it);
		 */
		/* 	printf("[%d.0] new zone tiny inside loop %p\n", count,
		 */
		/* 	       new_zone); */
		/* } */
		if (it->next == NULL) {
			it->next = new_zone;
			break;
		}
		it = it->next;
	}
	if (it == NULL)
		it = new_zone;
	/* count++; */
}

int new_zone(block_type_t type, size_t size)
{
	/* static int count = 0; */
	Zone *zone = (Zone *)mmap(NULL, size * BPZ, PROT_READ | PROT_WRITE,
	                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == NULL) {
		ft_dprintf(2, "error on syscall mmap\n");
		return (-1);
	}
	zone->type = type;
	zone->used = NULL;
	add_blocks(zone, BPZ, size);
	zone->next = NULL;

	append_zone(zone, type);
	/* if (type == TINY) { */
	/* 	printf("jdkslfa\n"); */
	/* 	for (Zone *zone_it = zones->tiny; zone_it != NULL; */
	/* 	     zone_it = zone_it->next) { */
	/* 		printf("[%d.1] zone tiny %p\n", count, zone_it); */
	/* 		printf("type: %d\n", type); */
	/* 	} */
	/* 	count++; */
	/* } */

	return (0);
}
