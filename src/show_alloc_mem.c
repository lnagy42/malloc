/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 17:58:11 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:12:05 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <unistd.h>

void	check_ifused_or_dbg(t_block *block, size_t *total)
{
	if (block->used)
	{
		ft_putaddr(block->data);
		ft_putstr(" - ");
		ft_putaddr(block->data + block->size);
		ft_putstr(" : ");
		ft_putsize_base(block->size, 10);
		ft_putstr(" octets | max: ");
		ft_putsize_base(block->max_size, 10);
		ft_putstr("\n");
		*total += block->size;
	}
	else if (DEBUG)
	{
		ft_putaddr(block->data);
		ft_putstr(" - ");
		ft_putaddr(block->data + block->size);
		ft_putstr(" : ");
		ft_putstr(" *freed* ");
		ft_putsize_base(block->max_size, 10);
		ft_putstr(" octets\n");
	}
}

void	put_region(t_block *beginlist_region, char *area, size_t *total)
{
	t_block	*block;

	while (beginlist_region)
	{
		ft_putstr(area);
		ft_putaddr(beginlist_region->data);
		put_size_dbg(beginlist_region->size);
		ft_putstr("\n");
		block = (t_block *)beginlist_region->data;
		while (block)
		{
			check_ifused_or_dbg(block, total);
			block = block->next;
		}
		beginlist_region = beginlist_region->next;
	}
}

void	mutex_error(char *s, int ret)
{
	if (DEBUG)
	{
		ft_putstr_fd(s, 2);
		ft_putsize_base((size_t)ret, 10);
	}
}

void	show_alloc_mem_thread_unsafe(void)
{
	size_t	total;

	total = 0;
	if (g_begin.tiny)
		put_region(g_begin.tiny, "TINY : ", &total);
	if (g_begin.small)
		put_region(g_begin.small, "SMALL : ", &total);
	if (g_begin.large)
		put_region(g_begin.large, "LARGE : ", &total);
	ft_putstr("Total : ");
	ft_putsize_base(total, 10);
	ft_putstr(" octets\n");
}

void	show_alloc_mem(void)
{
	int	ret;

	if ((ret = pthread_mutex_lock(&g_mutex)) != 0)
	{
		mutex_error("error mutex lock: ", ret);
		return ;
	}
	show_alloc_mem_thread_unsafe();
	if ((ret = pthread_mutex_unlock(&g_mutex)) != 0)
		mutex_error("error mutex unlock: ", ret);
}
