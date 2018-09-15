/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/13 17:46:16 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/15 13:25:37 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	*ft_realloc(void *ptr, size_t size)
{
	t_zone	current;
	t_zone	prev;
	void	*new_addr;
	size_t	left_region;

	put_request_realloc_dbg(ptr, size);
	size = align(size, PADDING);
	if (!ptr)
		return (ft_malloc(size));
	else if (size == 0)
	{
		ft_free(ptr);
		return(ft_malloc(0));
	}
	current = find_block(ptr, &prev);
	if (!current.block)
		return (put_ret_addr_dbg(NULL));
	if (size <= current.block->max_size)
	{
		current.block->size = size;
		return (put_ret_addr_dbg(ptr));
	}
	if (current.block->next && !current.block->next->used && (current.block->next->size + current.block->size + sizeof(t_block) >= size))
	{
		current.block->size = current.block->next->size + current.block->size + sizeof(t_block);
		current.block->max_size = current.block->size;
		current.block->next = current.block->next->next;
		return (put_ret_addr_dbg(ptr));
	}
	left_region = current.region->size - ((size_t)current.block->data - (size_t)current.region);
	if (!current.block->next && left_region >= size)
	{
		current.block->size = size;
		current.block->max_size = size;
		return (put_ret_addr_dbg(ptr));
	}
	new_addr = ft_malloc(size);
	new_addr = ft_memcpy(new_addr, ptr, current.block->size);
	ft_free(ptr);
	return (new_addr);
}