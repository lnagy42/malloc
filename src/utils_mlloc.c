/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mlloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 14:21:09 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 17:53:22 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

size_t	align(size_t size, size_t align)
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
	return ((((size - 1) >> shift) << shift) + align);
}

t_block	*create_block(size_t size, t_block *new_block)
{
	new_block->size = size;
	new_block->max_size = size;
	new_block->used = 1;
	new_block->next = NULL;
	return (new_block);
}

t_block	*init_region(t_block **region, size_t size, size_t region_size)
{
	if (!*region)
	{
		if ((*region = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_ANON
			| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		{
			ft_putstr_fd("MAP FAILED !\n", 2);
			return (NULL);
		}
		(*region)->size = region_size;
		(*region)->max_size = region_size;
		(*region)->used = 1;
		(*region)->next = NULL;
		create_block(size, (t_block *)(*region)->data);
		return (*region);
	}
	return (NULL);
}

t_block	*check_prevblock_pushback(size_t size, t_block *new_block,
	t_block *region, t_zone *prev)
{
	t_block	*end_region;
	size_t	size_left;

	end_region = 0;
	size_left = 0;
	if (prev->block)
	{
		end_region = (t_block *)(prev->block->data + prev->block->max_size);
		size_left = region->size - ((size_t)end_region - (size_t)region);
		if (size_left >= size + sizeof(t_block))
		{
			prev->block->next = end_region;
			prev->block->next->max_size = size;
			return (create_block(size, end_region));
		}
	}
	return (new_block);
}
