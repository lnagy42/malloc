/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 14:21:09 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:12:23 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

pthread_mutex_t g_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

t_block	*check_newblock_pushback(size_t size, t_block *new_block, t_zone *prev)
{
	while (new_block)
	{
		if (new_block->used == 0 && new_block->max_size >= size)
		{
			new_block->size = size;
			new_block->used = 1;
			return (new_block);
		}
		prev->block = new_block;
		new_block = new_block->next;
	}
	return (NULL);
}

t_block	**choose_list(size_t size, size_t *region_size)
{
	t_block	**begin_list;

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
		*region_size = align(size + 2 * sizeof(t_block), getpagesize());
	}
	return (begin_list);
}

t_block	*pushback_block(size_t size, size_t region_size, t_block *region)
{
	t_block	*new_block;
	t_zone	prev;

	prev.region = NULL;
	while (region)
	{
		prev.block = NULL;
		new_block = (t_block *)region->data;
		if ((new_block = check_newblock_pushback(size, new_block, &prev)))
			return (new_block);
		if ((new_block = check_prevblock_pushback(size, new_block, region,
			&prev)))
			return (new_block);
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
	if (size >= SIZE_MAX - 2 * sizeof(t_block))
		return (put_ret_addr_dbg(NULL));
	size = align(size, PADDING);
	list = choose_list(size, &region_size);
	if ((region = init_region(list, size, region_size)))
		return (put_ret_addr_dbg(((t_block *)region->data)->data));
	if ((block = pushback_block(size, region_size, *list)))
		return (put_ret_addr_dbg(block->data));
	return (put_ret_addr_dbg(NULL));
}

void	*malloc(size_t size)
{
	int		ret;
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
