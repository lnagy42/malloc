/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/30 19:15:38 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/13 18:33:13 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*tmp;
	char	*src_2;
	size_t	i;

	src_2 = (char *)src;
	tmp = (char *)dest;
	i = 0;
	while (i < n)
	{
		tmp[i] = src_2[i];
		i++;
	}
	return (dest);
}