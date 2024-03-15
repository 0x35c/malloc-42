#include "../includes/malloc.h"

// FULL_INFO is to display both used and unused blocks (and the nb)
#define FULL_INFO 1

#if FULL_INFO
static size_t get_nb_blocks(Block *head)
{
	size_t size = 0;
	for (Block *it = head; it != NULL; it = it->next) {
		size++;
	}
	return (size);
}
#endif

static bool zone_in_use(Block *head)
{
	for (Block *it = head; it != NULL; it = it->next)
		if (it->in_use)
			return (true);
	printf("Zone not in use\n");
	return (false);
}

void show_alloc_mem(void)
{
	Zone *const zones_map[3] = {zones->tiny, zones->small, zones->large};
	char *const zones_name[3] = {"TINY", "SMALL", "LARGE"};

	for (block_type_t type = 0; type < 3; ++type) {
		for (Zone *zone_it = zones_map[type]; zone_it != NULL;
		     zone_it = zone_it->next) {
			(void)zone_in_use(zone_it->used);
			/* if (zone_in_use(zone_it->used) == false) */
			/* 	continue; */
#if FULL_INFO
			ft_printf("%s : %p with %u blocks\n", zones_name[type],
			          zone_it, get_nb_blocks(zone_it->used));
#else
			ft_printf("%s : %p\n", zones_name[type], zone_it);
#endif
			for (Block *block_it = zone_it->free; block_it != NULL;
			     block_it = block_it->next) {
				ft_printf("%p - %p : %u bytes", block_it->ptr,
				          (size_t)block_it->ptr +
				              block_it->size,
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
}
