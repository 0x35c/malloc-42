#include "../includes/malloc.h"

block_type_t get_type(size_t size)
{
	if (size <= (size_t)PAGES_TINY * getpagesize() - sizeof(Block))
		return (TINY);
	if (size <= (size_t)PAGES_SMALL * getpagesize() - sizeof(Block))
		return (SMALL);
	return (LARGE);
}

Zone *get_zone(block_type_t type)
{
	if (type == TINY)
		return (zones->tiny);
	if (type == SMALL)
		return (zones->small);
	return (zones->large);
}

size_t get_max_size(block_type_t type)
{
	if (type == TINY)
		return (PAGES_TINY * getpagesize());
	if (type == SMALL)
		return (PAGES_SMALL * getpagesize());
	return (0);
}
