#include "../includes/malloc.h"

// FULL_INFO is to display both used and unused blocks (and the nb)
#define FULL_INFO 1

#if FULL_INFO
static size_t get_nb_blocks(Block *head)
{
	size_t size = 0;
	for (Block *it = head; it != NULL; it = it->next) {
		/* printf("it %p\n", it); */
		size++;
	}
	return (size);
}
#endif

void show_alloc_mem(void)
{
	for (Zone *zone_it = zones; zone_it != NULL; zone_it = zone_it->next) {
		if (zone_it->type == TINY) {
#if FULL_INFO
			ft_printf("TINY : %p with %u blocks\n", zone_it,
			          get_nb_blocks(zone_it->head));
#else
			ft_printf("TINY : %p\n", zone_it);
#endif
		} else if (zone_it->type == SMALL) {
#if FULL_INFO
			ft_printf("SMALL : %p with %u blocks\n", zone_it,
			          get_nb_blocks(zone_it->head));
#else
			ft_printf("SMALL : %p\n", zone_it);
#endif
		} else if (zone_it->type == LARGE)
			ft_printf("LARGE : %p\n", zone_it);
		for (Block *block_it = zone_it->head; block_it != NULL;
		     block_it = block_it->next) {
			ft_printf("%p - %p : %u bytes", block_it,
			          (size_t)block_it + block_it->size,
			          block_it->size);
#if FULL_INFO
			if (block_it->in_use == false)
				ft_printf(" (not in_use)\n");
			else
				ft_printf(" (in_use)\n");
#else
			if (block_it->in_use == true)
				ft_printf("\n");
#endif
		}
		ft_printf("------------------------------\n");
	}
}
