/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 17:59:10 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:00:22 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

t_zone	check_inblock(void *ptr, t_zone *prev, t_zone *current)
{
	while (current->region)
	{
		current->block = (t_block *)current->region->data;
		prev->block = NULL;
		while (current->block)
		{
			if (ptr == current->block->data && current->block->used)
				return (*current);
			prev->block = current->block;
			current->block = current->block->next;
		}
		prev->region = current->region;
		current->region = current->region->next;
	}
	return (*current);
}

void	init_zone(t_zone *current)
{
	current->region = NULL;
	current->block = NULL;
	current->begin_list = (t_block **)&g_begin;
}

t_zone	find_block(void *ptr, t_zone *prev)
{
	size_t	nb_list;
	size_t	i;
	t_zone	current;

	nb_list = sizeof(t_region) / sizeof(t_block *);
	i = 0;
	current.begin_list = (t_block **)&g_begin;
	current.block = NULL;
	init_zone(&current);
	while (i < nb_list)
	{
		current.region = (t_block *)*current.begin_list;
		prev->region = NULL;
		current = check_inblock(ptr, prev, &current);
		if (current.block)
			return (current);
		current.begin_list++;
		i++;
	}
	return (current);
}
