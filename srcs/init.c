#include "../includes/malloc.h"

Zones *zones;

int init_allocator(void)
{
	static bool initialized = false;
	if (initialized)
		return (0);
	initialized = true;

	int err;

	// We take 1 page to store the linked list for all the zones
	zones = (Zones *)mmap(NULL, sizeof(Zones), PROT_READ | PROT_WRITE,
	                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	zones->tiny = NULL;
	zones->small = NULL;
	zones->large = NULL;
	err = new_zone(TINY, getpagesize() * PAGES_TINY);
	if (err < 0)
		return (err);
	err = new_zone(SMALL, getpagesize() * PAGES_SMALL);
	if (err < 0)
		return (err);
	return (0);
}
