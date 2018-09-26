/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 14:21:09 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/20 15:07:44 by jfortin          ###   ########.fr       */
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
	size_t		nb_list;
	size_t		i;
	t_zone		current;

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
		// while (current.region)
		// {
		// 	current.block = (t_block *)current.region->data;
		// 	prev->block = NULL;
		// 	while (current.block)
		// 	{
		// 		if (ptr == current.block->data && current.block->used)
		// 			return (current);
		// 		prev->block = current.block;
		// 		current.block = current.block->next;
		// 	}
		// 	prev->region = current.region;
		// 	current.region = current.region->next;
		// }
		current.begin_list++;
		i++;
	}
	return (current);
}

void	free_region(t_zone *prev, t_zone *current)
{
	if (!prev->region)
		*current->begin_list = current->region->next;
	else
		prev->region->next = current->region->next;
	munmap(current->region, current->region->size);
	// ft_putstr("region freed\n");
}
	
void	free_last_block(t_zone *prev, t_zone *current) /*fonction de l'enfer*/
{
	if (prev->block)
	{
		if (prev->block->used == 0)
		{
			if (prev->block == (t_block *)current->region->data)
				free_region(prev, current);
			else
				prev->block->next = NULL;
		}
		else
			prev->block->next = NULL;
	}
	else
		free_region(prev, current);
}

void	check_ifprevused(t_zone *prev, t_zone *current)
{
	if (prev->block && !prev->block->used)
	{
		prev->block->max_size += current->block->max_size + sizeof(t_block);
		prev->block->size = prev->block->max_size;
		prev->block->next = current->block->next;
	}
}

void	ft_free_thread_unsafe(void *ptr)
{
	t_zone current;
	t_zone prev;

	put_request_free_dbg(ptr);
	if (ptr)
		current = find_block(ptr, &prev);
	if (!ptr || !current.block || !current.block->used)
		return;
	current.block->used = 0;
	if (!current.block->next)
		free_last_block(&prev, &current);
	else
	{
		if (current.block->next->used == 0)
		{
			current.block->max_size += current.block->next->max_size + sizeof(t_block);
			current.block->size = current.block->max_size;
			current.block->next = current.block->next->next;
			if (current.block->next == NULL && current.block == (t_block *)current.region->data)
				free_region(&prev, &current);
		}
		check_ifprevused(&prev, &current);
	}
}

void	free(void *ptr)
{
	int	ret;

	if ((ret = pthread_mutex_lock(&g_mutex)) != 0)
	{
		mutex_error("error mutex lock: ", ret);
		return ;
	}
	ft_free_thread_unsafe(ptr);
	if ((ret = pthread_mutex_unlock(&g_mutex)) != 0)
		mutex_error("error mutex unlock: ", ret);
}

