#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#include "../libs/libft/includes/libft.h"

// BPZ = Blocks Per Zone, which is the number
// of blocks allocated for a new zone
enum { BPZ = 8, PAGES_TINY = 4, PAGES_SMALL = 16, MEM_ALIGN = 16 };

/* Linked list to store all the zones (pages) mapped.
 * The attribute type is either TINY, SMALL or LARGE.
 * For TINY and SMALL, the zone will be divided in blocks.
 * For LARGE, it will be entire page(s).
 */
typedef enum { TINY, SMALL, LARGE } block_type_t;

/* next and prev will never change, it's its
 * position initialized when creating the blocks
 * next_used: the next in_use block
 * ptr: the ptr to return with malloc
 */
typedef struct Block {
	void *ptr;
	size_t size;
	bool in_use;
	struct Block *next;
	struct Block *prev;
	struct Block *next_used;
	struct Block *next_free;
} Block;

/* free is the first list, when creating the blocks
 * used is a list at the end of the free list, which contains
 * all the blocks in_use
 */
typedef struct Zone {
	struct Zone *next;
	block_type_t type;
	Block *free;
	Block *used;
} Zone;

typedef struct Zones {
	Zone *tiny;
	Zone *small;
	Zone *large;
} Zones;

extern Zones *zones;

/*-------- UTILS --------*/
block_type_t get_type(size_t size);
Zone *get_zone(block_type_t type);
size_t get_max_size(block_type_t type);
/* --------------------- */

/*--------  ALLOCATOR --------*/
int new_zone(block_type_t type, size_t block_size);
int init_allocator(void);

// CRINGE BOZO NATHAN
int getpagesize(void);

// Lib functions
void *malloc(size_t size);
void show_alloc_mem(void);
