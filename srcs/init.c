#include "../includes/malloc.h"
#include <stdint.h>
#include <unistd.h>

Zone *zones;

static void init_blocks(Zone *zone, size_t nb_blocks, size_t block_size)
{
	zone->head = (Block *)(zone + sizeof(Zone));
	Block *iter = zone->head;
	while ((uintptr_t)iter + block_size < (uintptr_t)zone + nb_blocks * block_size) {
		printf("zone %p - iter %p - block_size %ld\n", zone, iter, block_size / 32);
		iter->next = iter + (block_size / 32);
		iter->next->size = (block_size / 32) - sizeof(Block);
		iter->next->in_use = false;
		printf("zone %p - iter next %p - block_size %ld\n", zone, iter->next, block_size / 32);
		iter = iter->next;
	}
}

int init_allocator(void)
{
	static bool initialized = false;
	if (initialized)
		return (0);

	/* Size of each block (in bytes)
	 * /!\ This is not the real size, the real size is 
	 * of X_size - sizeof(Zone) 
	 * Which is the remaining size of the block we got
	 * with mmap once we added the metadata
	 */
	const size_t tiny_size = getpagesize() * PAGES_TINY;
	const size_t small_size = getpagesize() * PAGES_SMALL;

	// Init TINY zones with NB_BLOCKS_INIT blocks of 4 pages 
	Zone *tiny = (Zone *)mmap(NULL, tiny_size * NB_BLOCKS_INIT,
								PROT_READ | PROT_WRITE,
								MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (tiny == NULL) {
		ft_dprintf(2, "error on syscall mmap\n");
		return (-1);
	}
	zones = tiny;
	tiny->nb_pages = PAGES_TINY * NB_BLOCKS_INIT;
	tiny->type = TINY;
	init_blocks(tiny, NB_BLOCKS_INIT, tiny_size);

	// Init SMALL zones with NB_BLOCKS_INIT blocks of 16 pages, and set the TINY to point towards SMALL
	Zone *small = (Zone *)mmap(NULL, small_size * NB_BLOCKS_INIT,
								PROT_READ | PROT_WRITE,
								MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (small == NULL) {
		ft_dprintf(2, "error on syscall mmap\n");
		return (-1);
	}
	tiny->next = small;
	small->nb_pages = PAGES_SMALL * NB_BLOCKS_INIT;
	small->type = SMALL;
	small->next = NULL;
	init_blocks(small, NB_BLOCKS_INIT, small_size);
	return (0);
}
