#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

#define NB_BLOCKS_INIT 32
#define PAGES_TINY 4
#define PAGES_SMALL 16

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
	Block *head;
	block_type_t type;
	size_t nb_pages;
} Zone;

extern Zone *zones;

void *malloc(size_t size);
void show_alloc_mem(void);
int init_allocator(void);
//Zone *add_pages(Zone *pages, size_t nb, block_type_t type);
