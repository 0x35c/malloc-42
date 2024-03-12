#include "../includes/malloc.h"

void show_alloc_mem(void)
{
	for (Zone *zone_it = zones; zone_it->next != NULL; zone_it = zone_it->next) {
		if (zone_it->type == TINY)
			ft_printf("TINY : %p\n", zone_it);
		else if (zone_it->type == SMALL)
			ft_printf("SMALL : %p\n", zone_it);
		else if (zone_it->type == LARGE)
			ft_printf("LARGE : %p\n", zone_it);
		for (Block *block_it = zone_it->head; block_it->next != NULL; block_it = block_it->next) {
			ft_printf("%p - %p : %u bytes ", block_it, (size_t)block_it + block_it->size, block_it->size);
			if (block_it->in_use == false)
				ft_printf("(not in_use)\n");
			else
				ft_printf("(in_use)\n");
		}
	}
}
