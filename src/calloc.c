/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 14:21:09 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:16:10 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <string.h>

void	*ft_calloc_thread_unsafe(size_t count, size_t size)
{
	void	*addr;

	if (DEBUG)
		ft_putstr("Request calloc -> ");
	if (!(addr = ft_malloc_thread_unsafe(count * size)))
		return (NULL);
	return (memset(addr, 0, align(count * size, PADDING)));
}

void	*calloc(size_t count, size_t size)
{
	int		ret;
	t_block	*addr;

	if ((ret = pthread_mutex_lock(&g_mutex)) != 0)
	{
		mutex_error("error mutex lock: ", ret);
		return (NULL);
	}
	addr = ft_calloc_thread_unsafe(count, size);
	if ((ret = pthread_mutex_unlock(&g_mutex)) != 0)
		mutex_error("error mutex unlock: ", ret);
	return (addr);
}
