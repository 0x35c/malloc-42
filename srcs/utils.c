#include "malloc.h"

static size_t get_block_size(block_type_t type)
{
	if (type == TINY)
		return ((PAGES_TINY * getpagesize()) / BPZ -
		        align_mem(sizeof(Block)));
	if (type == SMALL)
		return ((PAGES_SMALL * getpagesize()) / BPZ -
		        align_mem(sizeof(Block)));
	return (0);
}

block_type_t get_type(size_t size)
{
	if (size <= get_block_size(TINY))
		return (TINY);
	if (size <= get_block_size(SMALL))
		return (SMALL);
	return (LARGE);
}

size_t get_zone_size(block_type_t type)
{
	if (type == TINY)
		return (PAGES_TINY * getpagesize());
	if (type == SMALL)
		return (PAGES_SMALL * getpagesize());
	return (0);
}

size_t align_mem(size_t addr)
{
	return (addr + (MEM_ALIGN - 1)) & ~(MEM_ALIGN - 1);
}
