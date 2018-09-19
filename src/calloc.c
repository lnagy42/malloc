/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/19 14:47:31 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/19 17:10:26 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <string.h>

void	*calloc(size_t count, size_t size)
{
	void	*addr;

	if (DEBUG)
		ft_putstr("Request calloc -> ");
	if (!(addr = ft_malloc_thread_unsafe(count * size)))
		return (NULL);
	return (memset(addr, 0, align(count * size, PADDING)));
}