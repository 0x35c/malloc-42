#include "../includes/malloc.h"

size_t get_nb_blocks(Block *head) {
	size_t size = 0;
	for (Block *it = head; it != NULL; it = it->next) {
		/* printf("iter addr %p\n", it); */
		size++;
	}
	return (size);
}

void show_alloc_mem(void)
{
	for (Zone *zone_it = zones; zone_it != NULL; zone_it = zone_it->next) {
		if (zone_it->type == TINY)
			ft_printf("TINY : %p with %u blocks\n", zone_it, get_nb_blocks(zone_it->head));
		else if (zone_it->type == SMALL)
			ft_printf("SMALL : %p with %u blocks\n", zone_it, get_nb_blocks(zone_it->head));
		else if (zone_it->type == LARGE)
			ft_printf("LARGE : %p\n", zone_it);
		for (Block *block_it = zone_it->head; block_it != NULL; block_it = block_it->next) {
			ft_printf("%p - %p : %u bytes ", block_it, (size_t)block_it + block_it->size, block_it->size);
			if (block_it->in_use == false)
				ft_printf("(not in_use)\n");
			else
				ft_printf("(in_use)\n");
		}
		ft_printf("------------------------------\n");
	}
}
