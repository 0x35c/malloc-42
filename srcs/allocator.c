#include "../includes/malloc.h"

Zones *zones;

static void add_blocks(Zone *zone, size_t nb_blocks, size_t block_size)
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
	Zone **begin;
	if (type == TINY)
		begin = &zones->tiny;
	else if (type == SMALL)
		begin = &zones->small;
	else
		begin = &zones->large;

	// If the list is empty, put new_zone at the beginning
	if (*begin == NULL) {
		*begin = new_zone;
		return;
	}

	Zone *it = *begin;
	while (it->next)
		it = it->next;
	it->next = new_zone;
}

int new_zone(block_type_t type, size_t block_size)
{
	struct rlimit limit;
	if (getrlimit(RLIMIT_AS, &limit) == -1) {
		ft_dprintf(2, "error: syscall getrlimit failed\n");
		return (-1);
	}
	if (block_size * BPZ >= limit.rlim_max) {
		ft_dprintf(2, "error: no more memory available\n");
		return (-1);
	}

	Zone *zone =
	    (Zone *)mmap(NULL, block_size * BPZ, PROT_READ | PROT_WRITE,
	                 MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (zone == NULL) {
		ft_dprintf(2, "error: syscall mmap failed\n");
		return (-1);
	}
	zone->type = type;
	zone->used = NULL;
	add_blocks(zone, BPZ, block_size);
	zone->next = NULL;

	append_zone(zone, type);

	return (0);
}

int init_allocator(void)
{
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
	err = new_zone(TINY, getpagesize() * PAGES_TINY);
	if (err < 0)
		return (err);
	err = new_zone(SMALL, getpagesize() * PAGES_SMALL);
	if (err < 0)
		return (err);
	return (0);
}