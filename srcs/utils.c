#include "../includes/malloc.h"

block_type_t find_type(size_t size)
{
	if (size <= (size_t)PAGES_TINY * getpagesize() - sizeof(Block))
		return (TINY);
	if (size <= (size_t)PAGES_SMALL * getpagesize() - sizeof(Block))
		return (SMALL);
	return (LARGE);
}
