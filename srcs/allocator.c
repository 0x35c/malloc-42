#include "../includes/malloc.h"
#include <unistd.h>

Zones *zones;

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
	new_block->ptr = (Block *)align_mem((size_t)new_block + sizeof(Block));
	new_block->zone = zone;

	// Init future linked lists
	new_block->prev = NULL;
	new_block->prev_free = NULL;
	new_block->prev_used = NULL;
	new_block->next = NULL;
	new_block->next_free = NULL;
	new_block->next_used = NULL;

	if (zone->free) {
		zone->free->prev = new_block;
		zone->free->prev_free = new_block;
		new_block->next_free = zone->free;
		new_block->next = zone->free;
	}
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
		if (count > 0 && count < nb_zones)
			prev->next = new_zone;
		prev = new_zone;
		new_zone->type = type;
		new_zone->nb_blocks = 1;
		new_zone->used = NULL;
		new_zone->next = NULL;
		new_block(new_zone, zone_size);
	}

	append_list(heap, type);

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

	err = new_zones(TINY, PAGES_TINY * getpagesize(), INIT_ZONES);
	if (err < 0)
		return (err);
	err = new_zones(SMALL, PAGES_SMALL * getpagesize(), INIT_ZONES);
	if (err < 0)
		return (err);
	return (0);
}
