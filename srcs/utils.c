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
	const size_t zone_size =
	    (size_t)zone + nb_blocks * block_size + sizeof(Zone);
	zone->head = (Block *)((size_t)zone + sizeof(Zone));
	Block *iter = zone->head;
	do {
		iter->size = block_size - sizeof(Block);
		iter->in_use = false;
		iter->next = (Block *)((size_t)iter + block_size);
		iter = iter->next;
	} while ((size_t)iter + block_size <= zone_size);
	Block *prev = (Block *)((size_t)iter - block_size);
	prev->next = NULL;
	iter->next = NULL;
}

int add_zone(block_type_t type, size_t size)
{
	static int count = 0;
	Zone *zone = (Zone *)mmap(NULL, size * BPZ + sizeof(Zone),
	                          PROT_READ | PROT_WRITE,
	                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == NULL) {
		ft_dprintf(2, "error on syscall mmap\n");
		return (-1);
	}
	if (type == TINY) {
		zone->nb_pages = PAGES_TINY * BPZ;
		zone->type = TINY;
		add_blocks(zone, BPZ, size);
	}
	if (type == SMALL) {
		zone->nb_pages = PAGES_SMALL * BPZ;
		zone->type = SMALL;
		add_blocks(zone, BPZ, size);
	}
	zone->next = NULL;

	// Add the new zone to the end of the Zones
	Zone **it = NULL;
	if (type == TINY) {
		it = &zones->tiny;
	} else if (type == SMALL) {
		it = &zones->small;
	} else {
		it = &zones->large;
	}
	Zone **tmp = it;
	while (*tmp) {
		if (type == TINY) {
			printf("[%d.0] zone tiny inside loop %p\n", count,
			       *tmp);
			printf("[%d.0] new zone tiny inside loop %p\n", count,
			       zone);
		}
		*tmp = (*tmp)->next;
	}
	*tmp = zone;
	if (type == TINY) {
		for (Zone *zone_it = zones->tiny; zone_it;
		     zone_it = zone_it->next) {
			printf("[%d.1] zone tiny %p\n", count, zone_it);
			printf("[%d.1] zone tiny next %p\n", count,
			       zone_it->next);
		}
		count++;
	}

	return (0);
}
