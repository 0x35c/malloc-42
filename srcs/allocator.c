#include "malloc.h"

Zone *zones[3];

static void add_zone(Zone *zone, block_type_t type)
{
	// We put the zone at the beginning of the list
	if (zones[type]) {
		zone->next = zones[type];
		zones[type]->prev = zone;
	}
	zones[type] = zone;
}

static void new_block(Zone *zone, size_t zone_size)
{
	Block *new_block = (Block *)align_mem((size_t)zone + sizeof(Zone));

	// Metadata
	new_block->in_use = false;
	new_block->size = zone_size - sizeof(Zone) - sizeof(Block);
	new_block->sub_size = new_block->size;
	new_block->ptr = (Block *)((size_t)new_block + sizeof(Block));
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
		new_block->next = zone->free;
		new_block->next_free = zone->free;
	}
	zone->free = new_block;
}

int new_zone(block_type_t type, size_t size)
{
	struct rlimit limit;
	if (getrlimit(RLIMIT_AS, &limit) == -1) {
		ft_dprintf(2, "error: syscall getrlimit failed\n");
		return (-1);
	}
	if (size >= limit.rlim_max) {
		ft_dprintf(2, "error: no more memory available\n");
		return (-1);
	}

	void *heap = mmap(NULL, size, PROT_READ | PROT_WRITE,
	                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (heap == NULL) {
		ft_dprintf(2, "error: syscall mmap failed\n");
		return (-1);
	}

	Zone *zone = (Zone *)heap;
	zone->type = type;
	zone->size = size;
	zone->used = NULL;
	zone->next = NULL;
	zone->prev = NULL;

	new_block(zone, size);
	add_zone(heap, type);

	return (0);
}
