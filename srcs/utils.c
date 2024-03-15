#include "../includes/malloc.h"

block_type_t get_type(size_t size)
{
	if (size <= get_max_size(TINY))
		return (TINY);
	if (size <= get_max_size(SMALL))
		return (SMALL);
	return (LARGE);
}

Zone *get_zone(block_type_t type)
{
	if (type == TINY)
		return (zones->tiny);
	if (type == SMALL)
		return (zones->small);
	return (NULL);
}

size_t get_max_size(block_type_t type)
{
	if (type == TINY)
		return (PAGES_TINY * getpagesize() - sizeof(Block));
	if (type == SMALL)
		return (PAGES_SMALL * getpagesize() - sizeof(Block));
	return (0);
}
