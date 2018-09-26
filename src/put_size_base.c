/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_size_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 18:02:38 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:02:58 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	ft_putsize_base(size_t size, unsigned int base)
{
	char				buff[66];
	static const char	table[16] = "0123456789ABCDEF";
	size_t				power;
	int					i;

	power = ft_size(size, base);
	i = 0;
	if (base == 16)
	{
		buff[0] = '0';
		buff[1] = 'x';
		i = 2;
	}
	while (power)
	{
		buff[i++] = table[size / power];
		size -= (size / power) * power;
		power /= base;
	}
	buff[i] = '\0';
	ft_putstr(buff);
}
