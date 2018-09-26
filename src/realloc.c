/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 17:46:16 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/26 18:44:23 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	*check_left_size(void *ptr, size_t size, t_zone *current)
{
	size_t	left_region;

	left_region = current->region->size -
		((size_t)current->block->data - (size_t)current->region);
	if (!current->block->next && left_region >= size)
	{
		current->block->size = size;
		current->block->max_size = size;
		return (put_ret_addr_dbg(ptr));
	}
	return (put_ret_addr_dbg(NULL));
}

void	*check_size_n_used(void *ptr, size_t size, t_zone *current)
{
	size = align(size, PADDING);
	if (size <= current->block->max_size)
	{
		current->block->size = size;
		return (put_ret_addr_dbg(ptr));
	}
	if (current->block->next && !current->block->next->used &&
		(current->block->next->size + current->block->size
		+ sizeof(t_block) >= size))
	{
		current->block->size = current->block->next->size + current->block->size
			+ sizeof(t_block);
		current->block->max_size = current->block->size;
		current->block->next = current->block->next->next;
		return (put_ret_addr_dbg(ptr));
	}
	if (ptr == check_left_size(ptr, size, current))
		return (put_ret_addr_dbg(ptr));
	return (put_ret_addr_dbg(NULL));
}

void	*ft_realloc_thread_unsafe(void *ptr, size_t size)
{
	t_zone	current;
	t_zone	prev;
	void	*new_addr;

	put_request_realloc_dbg(ptr, size);
	if (!ptr)
		return (ft_malloc_thread_unsafe(size));
	else if (size == 0)
	{
		ft_free_thread_unsafe(ptr);
		return (ft_malloc_thread_unsafe(0));
	}
	current = find_block(ptr, &prev);
	if (!current.block)
		return (put_ret_addr_dbg(NULL));
	if (ptr == check_size_n_used(ptr, size, &current))
		return (put_ret_addr_dbg(ptr));
	new_addr = ft_malloc_thread_unsafe(size);
	new_addr = ft_memcpy(new_addr, ptr, current.block->size);
	ft_free_thread_unsafe(ptr);
	return (new_addr);
}

void	*realloc(void *ptr, size_t size)
{
	int		ret;
	t_block	*addr;

	if ((ret = pthread_mutex_lock(&g_mutex)) != 0)
	{
		mutex_error("error mutex lock: ", ret);
		return (NULL);
	}
	addr = ft_realloc_thread_unsafe(ptr, size);
	if ((ret = pthread_mutex_unlock(&g_mutex)) != 0)
		mutex_error("error mutex unlock: ", ret);
	return (addr);
}
