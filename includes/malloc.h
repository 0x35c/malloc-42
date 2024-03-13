#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../libs/libft/includes/libft.h"

// BPZ = Blocks Per Zone, which is the number
// of blocks allocated for a new zone
enum { BPZ = 32, PAGES_TINY = 4, PAGES_SMALL = 16 };

/* Linked list to store all the zones (pages) mapped.
 * The attribute type is either TINY, SMALL or LARGE.
 * For TINY and SMALL, the zone will be divided in blocks.
 * For LARGE, it will be entire page(s).
 */
typedef enum { TINY, SMALL, LARGE } block_type_t;

typedef struct Block {
	struct Block *next;
	size_t size;
	bool in_use;
} Block;

typedef struct Zone {
	struct Zone *next;
	block_type_t type;
	size_t nb_pages;
	Block *head;
} Zone;

typedef struct Zones {
	Zone *tiny;
	Zone *small;
	Zone *large;
} Zones;

extern Zones *zones;

block_type_t get_type(size_t size);
Zone *get_zone(block_type_t type);
size_t get_max_size(block_type_t type);
void add_blocks(Zone *zone, size_t nb_blocks, size_t block_size);
int add_zone(block_type_t type, size_t size);

int init_allocator(void);

void *malloc(size_t size);
void show_alloc_mem(void);
