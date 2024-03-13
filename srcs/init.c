#include "../includes/malloc.h"

Zone *zones;

static void init_blocks(Zone *zone, size_t nb_blocks, size_t block_size)
{
	const size_t zone_size = (size_t)zone + nb_blocks * block_size + sizeof(Zone);
	zone->head = (Block *)((size_t)zone + sizeof(Zone));
	Block *iter = zone->head;
	do {
		iter->size = block_size - sizeof(Block);
		iter->in_use = false;
		iter->next = (Block *)((size_t)iter + block_size);
		iter = iter->next;
		/* printf("iter size_t: %ld - iter ptr: %p\n", (size_t)iter, iter); */
		/* printf("zone %p - iter %p - block_size %ld\n", zone, iter, block_size); */
		/* printf("sum of iter + block_size: %p\n", iter + block_size); */
		/* printf("zone %p - iter->next %p - block_size %ld\n", zone, iter + block_size,  block_size); */
		/* printf("New block here %p\n", iter); */
	} while ((size_t)iter + block_size <= zone_size);
	Block *prev = (Block *)((size_t)iter - block_size);
	prev->next = NULL;
}

int init_allocator(void)
{
	static bool initialized = false;
	if (initialized)
		return (0);

	/* Size of each block (in bytes)
	 * /!\ This is not the real size, the real size is 
	 * of X_size - sizeof(Block) 
	 * Which is the remaining size of the block
	 * once we added the metadata
	 */
	const size_t tiny_size = getpagesize() * PAGES_TINY;
	const size_t small_size = getpagesize() * PAGES_SMALL;

	// Init TINY zones with NB_BLOCKS_INIT blocks of 4 pages 
	Zone *tiny = (Zone *)mmap(NULL, tiny_size * NB_BLOCKS_INIT + sizeof(Zone),
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
	Zone *small = (Zone *)mmap(NULL, small_size * NB_BLOCKS_INIT + sizeof(Zone),
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
