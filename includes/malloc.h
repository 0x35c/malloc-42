#pragma once

// pthread_mutex_*
#include <pthread.h>

// boolean types
#include <stdbool.h>

// size_t, already in libft.h but for readability
#include <stddef.h>

// mmap/unmap and getrlimit()
#include <sys/mman.h>
#include <sys/resource.h>

// getpagesize()
#include <unistd.h>

#include "../libs/libft/includes/libft.h"

// Remove this and replace it with <assert.h> header
// for debugging purposes
#define assert(bool)

// BPZ = Blocks Per Zone, which is the max
// number of blocks for a new zone
enum { BPZ = 128, PAGES_TINY = 16, PAGES_SMALL = 64, MEM_ALIGN = 8 };

typedef enum { TINY, SMALL, LARGE } block_type_t;

/* METADATA:
 * ptr: the ptr to return with malloc (aligned)
 * size: the actual size
 * sub_size: the size asked by the user (different
 * from size only if realloc and realloc size < size)
 * in_use: bool to track block state
 * zone: the zone containing the block
 *
 * LINKED LIST:
 * next and prev will never change, it's the original block's
 * position (initialized when creating the blocks)
 * next/prev_used: linked list for the
 * in_use blocks (Block *used in struct Zone)
 * next/prev_free: linked list for the
 * available blocks (Block *free in struct Zone)
 */
typedef struct Block {

	void *ptr;
	size_t size;
	size_t sub_size;
	bool in_use;
	struct Zone *zone;

	struct Block *prev;
	struct Block *next;
	struct Block *prev_used;
	struct Block *next_used;
	struct Block *prev_free;
	struct Block *next_free;
} Block;

/* free is the first list, when creating the blocks
 * used is a list at the end of the free list, which contains all the blocks
 * in_use
 */
typedef struct Zone {
	struct Zone *prev;
	struct Zone *next;
	block_type_t type;
	Block *free;
	Block *used;
} Zone;

/* Linked list to store all the zones (pages) mapped.
 * The attribute type is either TINY, SMALL or LARGE.
 * For TINY and SMALL, the zone will be divided in blocks.
 * For LARGE, it will be entire page(s).
 */
extern Zone *zones[3];

// Mutex used to make the allocator "thread-safe"
extern pthread_mutex_t g_thread_safe;

/*-------- UTILS --------*/
block_type_t get_type(size_t size);
size_t get_max_size(block_type_t type);
size_t get_zone_size(block_type_t type);
size_t align_mem(size_t addr);
/*-----------------------*/

/*-------- ALLOCATOR ---------*/
int new_zone(block_type_t type, size_t block_size);
/*----------------------------*/

/*-------- LIB ---------*/
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);
/*----------------------*/
