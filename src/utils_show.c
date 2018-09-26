/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_show.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 17:54:00 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:08:43 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <unistd.h>

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

size_t	ft_size(size_t size, unsigned int base)
{
	size_t	power;

	power = 1;
	while (size /= base)
		power *= base;
	return (power);
}

void	ft_putaddr(void *addr)
{
	ft_putsize_base((size_t)addr, 16);
}
