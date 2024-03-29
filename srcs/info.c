#include "malloc.h"

// FULL_INFO is to display (or not) both used and unused blocks
#define FULL_INFO 0

void show_alloc_mem(void)
{
	pthread_mutex_lock(&g_thread_safe);
	char *const zones_name[3] = {"TINY", "SMALL", "LARGE"};
	size_t total_size = 0;

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
				total_size += block_it->sub_size;
			}
			if (zone_it->used)
				ft_printf("\n");
			count++;
		}
	}
	ft_printf("Total: %u\n", total_size);
	pthread_mutex_unlock(&g_thread_safe);
}
