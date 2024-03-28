#include "../includes/malloc.h"

block_type_t get_type(size_t size)
{
	if (size <= get_max_size(TINY))
		return (TINY);
	if (size <= get_max_size(SMALL))
		return (SMALL);
	return (LARGE);
}

size_t get_max_size(block_type_t type)
{
	if (type == TINY)
		return ((PAGES_TINY * getpagesize()) / BPZ - sizeof(Block));
	if (type == SMALL)
		return ((PAGES_SMALL * getpagesize()) / BPZ - sizeof(Block));
	return (0);
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
