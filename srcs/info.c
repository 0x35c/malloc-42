#include "../includes/malloc.h"

// FULL_INFO is to display both used and unused blocks (and the nb)
#define FULL_INFO 0

void show_alloc_mem(void)
{
	init_allocator();
	char *const zones_name[3] = {"TINY", "SMALL", "LARGE"};

	for (block_type_t type = 0; type < 3; ++type) {
		int count = 0;
		for (Zone *zone_it = zones[type]; zone_it != NULL;
		     zone_it = zone_it->next) {
#if FULL_INFO
			if (zone_it->free)
				ft_printf("---------- AVAILABLE %s [n°%d - %p] "
				          "----------\n",
				          zones_name[type], count, zone_it);
			for (Block *block_it = zone_it->free; block_it != NULL;
			     block_it = block_it->next_free) {
				ft_printf("%p - %p : %u bytes\n", block_it->ptr,
				          (size_t)block_it->ptr +
				              block_it->sub_size +
				              sizeof(Block),
				          block_it->sub_size);
			}
			if (zone_it->free)
				ft_printf("\n");
#endif
			if (zone_it->used)
				ft_printf("---------- IN_USE %s [n°%d - %p] "
				          "----------\n",
				          zones_name[type], count, zone_it);
			for (Block *block_it = zone_it->used; block_it != NULL;
			     block_it = block_it->next_used) {
				ft_printf("%p - %p : %u bytes\n", block_it->ptr,
				          (size_t)block_it->ptr +
				              block_it->sub_size +
				              sizeof(Block),
				          block_it->sub_size);
			}
			if (zone_it->used)
				ft_printf("\n");
			count++;
		}
	}
}

/* #if FULL_INFO */
/* 			ft_printf("%s : %p with %u
 * blocks\n", zones_name[type], */
/* 			          zone_it,
 * get_nb_blocks(zone_it->head)); */
/* #else */
/* 			ft_printf("%s : %p\n",
 * zones_name[type], zone_it); */
/* #endif */
/* for (Block *block_it = zone_it->head; block_it !=
 * NULL; */
/*      block_it = block_it->next) { */
/* 	ft_printf("%p - %p : %u bytes", block_it->ptr,
 */
/* 	          (size_t)block_it->ptr + */
/* 	              block_it->sub_size, */
/* 	          block_it->sub_size); */
/* #if FULL_INFO */
/* 	if (block_it->in_use == false) */
/* 		ft_printf(" (not in_use)\n"); */
/* 	else */
/* 		ft_printf(" (in_use)\n"); */
/* #else */
/* 	if (block_it->in_use == true) */
/* 		ft_printf("\n"); */
/* #endif */
/* } */
