/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 16:03:03 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/13 19:29:37 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	put_request_malloc_dbg(size_t size)
{
	if (DEBUG)
	{
		ft_putstr("Request malloc with size: ");
		ft_putsize_base(size, 10);
	}
}

void	put_request_free_dbg(void *addr)
{
	if (DEBUG)
	{
		ft_putstr("Request free with addr: ");
		ft_putaddr(addr);
		ft_putstr("\n");
	}
}

void	put_request_realloc_dbg(void *addr, size_t size)
{
	if (DEBUG)
	{
		ft_putstr("Request realloc with addr: ");
		ft_putaddr(addr);
		ft_putstr(" and size: ");
		ft_putsize_base(size, 10);
		ft_putstr("\n");
	}
}

void	*put_ret_addr_dbg(void *addr)
{
	if (DEBUG)
	{
		ft_putstr(" return: ");
		ft_putaddr(addr);
		ft_putstr("\n");
	}
	return (addr);
}

void	put_size_dbg(size_t size)
{
	if (DEBUG)
	{
		ft_putstr("    size: ");
		ft_putsize_base(size, 10);
	}
}