#include "../inc/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

pthread_mutex_t g_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

size_t		align(size_t size, size_t align)
{
	size_t	shift;
	size_t	temp;

	if (size <= align)
		return (align);
	temp = align;
	shift = 0;
	while (temp >= 2)
	{
		shift++;
		temp >>= 1;
	}
	/* decalage binaire droite ">>" == "diviser par multiple de 2". shift == calculer l'alignement (1 en binaire == /2)*/
	return ((((size - 1) >> shift) << shift) + align);
}

t_block		**choose_list(size_t size, size_t *region_size)
{
	t_block	**begin_list;
	// t_block	*begin_region;

	if (size <= 2048)
	{
		begin_list = &g_begin.tiny;
		*region_size = align(2048 * 100, getpagesize());
	}
	else if (size > 2048 && size <= 16384)
	{
		begin_list = &g_begin.small;
		*region_size = align(16384 * 100, getpagesize());
	}
	else
	{
		begin_list = &g_begin.large;
		*region_size = align(size, getpagesize());
	}
	return (begin_list);
}

t_block		*create_block(size_t size, t_block *new_block)
{
	new_block->size = size;
	new_block->max_size = size;
	new_block->used = 1;
	new_block->next = NULL;
	return (new_block);
}

t_block		*init_region(t_block **region, size_t size, size_t region_size)
{
	// t_block	*init_block;

	if (!*region)
	{
		if ((*region = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		{
			ft_putstr("MAP FAILED !\n");
			return (NULL);
		}
		(*region)->size = region_size;
		(*region)->used = 1;
		(*region)->next = NULL;
		create_block(size, (t_block *)(*region)->data);
		return (*region);
	}
	return (NULL);
}

t_block	*pushback_block(size_t size, size_t region_size, t_block *region)
{
	t_block	*new_block;
	t_block	*end_region;
	t_zone	prev;
	size_t	size_left;

	prev.region = NULL;
	while (region)
	{
		prev.block = NULL;
		new_block = (t_block *)region->data;
		while (new_block)
		{
			if (new_block->used == 0 && new_block->max_size >= size)
			{
				new_block->size = size;
				// new_block->max_size = size;
				new_block->used = 1;
				return (new_block);
			}
			prev.block = new_block;
			new_block = new_block->next;
		}
		if (prev.block)
		{
			end_region = (t_block *)(prev.block->data + prev.block->max_size);
			size_left = region->size - ((size_t)end_region - (size_t)region);
			if (size_left >= size + sizeof(t_block))
			{
				prev.block->next = end_region;
				prev.block->next->max_size = size;
				return (create_block(size, end_region));
			}
		}
		prev.region = region;
		region = region->next;
	}
	if (!init_region(&prev.region->next, size, region_size))
		return (NULL);
	return ((t_block *)prev.region->next->data);
}

void	*ft_malloc_thread_unsafe(size_t size)
{
	t_block	**list;
	t_block	*region;
	t_block	*block;
	size_t	region_size;

	put_request_malloc_dbg(size);
	size = align(size, PADDING);
	list = choose_list(size, &region_size);
	if ((region = init_region(list, size, region_size)))
		return (put_ret_addr_dbg(((t_block *)region->data)->data));
	if ((block = pushback_block(size, region_size, *list)))
		return (put_ret_addr_dbg(block->data));
	return (put_ret_addr_dbg(NULL));
}

void	*ft_malloc(size_t size)
{
	int	ret;
	t_block	*addr;

	if ((ret = pthread_mutex_lock(&g_mutex)) != 0)
	{
		mutex_error("error mutex lock: ", ret);
		return (NULL);
	}
	addr = ft_malloc_thread_unsafe(size);
	if ((ret = pthread_mutex_unlock(&g_mutex)) != 0)
		mutex_error("error mutex unlock: ", ret);
	return (addr);
}
