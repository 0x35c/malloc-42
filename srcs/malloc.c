#include "../includes/malloc.h"
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

/*
static size_t offset_tiny(void) {
	return (getpagesize() / 16);
}

static size_t offset_small(void) {
	return (getpagesize() / 32);
}

static Zone *add_new_zone(block_type_t type)
{
	Zone *new_zone = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
							MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	new_zone->next = NULL;
	new_zone->type = type;
	new_zone->available = (uint8_t *)(&new_zone + sizeof(Zone));
	if (type == TINY) {
		for (size_t i = 0; i < offset_tiny(); ++i)
			new_zone->available[i] = i;
		new_zone->data = (uint8_t *)(&new_zone + sizeof(Zone) + offset_tiny());
	}
	else if (type == SMALL) {
		for (size_t i = 0; i < offset_small(); ++i)
			new_zone->available[i] = i;
		new_zone->data = (uint8_t *)(&new_zone + sizeof(Zone) + offset_small());
	}
	return (new_zone);
}

Zone *add_pages(Zone *pages, size_t nb, block_type_t type)
{
	Zone *head = pages;
	while (pages->next != NULL)
		pages = pages->next;
	for (size_t i = 0; i < nb; ++i) {
		pages->next = add_new_zone(type);
		pages = pages->next;
	}
	return (head);
}
*/

void *malloc(size_t size)
{
	init_allocator();
	block_type_t type;

	if (size <= TINY)
		type = TINY;	
	else if (size <= SMALL)
		type = SMALL;
	else
		type = LARGE;
	return (zones);
}

