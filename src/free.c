/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 14:21:09 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:14:13 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	free_region(t_zone *prev, t_zone *current)
{
	if (!prev->region)
		*current->begin_list = current->region->next;
	else
		prev->region->next = current->region->next;
	munmap(current->region, current->region->size);
}

void	free_last_block(t_zone *prev, t_zone *current)
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
		return ;
	current.block->used = 0;
	if (!current.block->next)
		free_last_block(&prev, &current);
	else
	{
		if (current.block->next->used == 0)
		{
			current.block->max_size += current.block->next->max_size
				+ sizeof(t_block);
			current.block->size = current.block->max_size;
			current.block->next = current.block->next->next;
			if (current.block->next == NULL && current.block
				== (t_block *)current.region->data)
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
