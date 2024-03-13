#include "../includes/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

/* Find first available (not in_use) block
 * in a zone matching the size we need
 */
static Block *find_block(block_type_t type, size_t size)
{
	for (Zone *zone_it = zones; zone_it != NULL; zone_it = zone_it->next) {
		if (zone_it->type == type) {
			for (Block *block_it = zone_it->head; block_it != NULL;
			     block_it = block_it->next) {
				if (block_it->in_use == false &&
				    block_it->size >= size)
					return (block_it);
			}
		}
	}
	return (NULL);
}

/* This will split the newly allocated block to use
 * the remaining bytes for a new block
 * This is our linked list of blocks
 * ... -> [5] -> [6] -> ...
 * After the allocation, this will become
 * ... -> [5] -> [new] -> [6] -> ...
 *
 * For an example of [5].size = 32 and requiring a malloc of 10
 * Let's say the metadata takes a size of 2:
 * ... -> [metadata][data][remaining size] -> [6]
 *            ^        ^         ^
 *            2       10        20
 *
 * So now our block [new] will become:
 * [5] -> [metadata][available data] -> [6]
 *             ^           ^
 *             2          18
 * We can see that it now has its own metadata and available
 * data and it points towards [6]
 */
static void frag_block(Block *left_block, size_t left_size)
{
	if (left_block->size - left_size < sizeof(Block))
		return;
	Block *right_block = (Block *)((size_t)left_block + left_size);
	right_block->size = left_block->size - left_size;
	right_block->in_use = false;
	right_block->next = left_block->next;
	left_block->next = right_block;
	/* printf("right_block addr %p\n", right_block); */
	/* printf("right_block->next addr %p\n", right_block->next); */
	/* printf("tmp->next addr %p\n", tmp->next); */
}

void *malloc(size_t size)
{
	// If mmap fails, the allocator won't be able to init correctly
	if (init_allocator() == -1) {
		ft_dprintf(2, "couldn't init allocator\n");
		return (NULL);
	}

	// Find the type of zone we need to search for
	block_type_t type = find_type(size);

	// Find an available block in a zone of type "type"
	Block *available = find_block(type, size);
	if (available == NULL)
		return (NULL); // We need more zones, TODO

	frag_block(available, size + sizeof(Block));
	available->in_use = true;
	available->size = size;
	void *ptr = (void *)((size_t)available + sizeof(Block));
	return (ptr);
}
