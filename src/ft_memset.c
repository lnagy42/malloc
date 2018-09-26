/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/19 15:27:51 by jfortin           #+#    #+#             */
/*   Updated: 2018/09/26 18:15:11 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

void	*ft_memset1(void *s, int c, size_t n)
{
	while (n > 0)
	{
		((unsigned char *)s)[n - 1] = (unsigned char)c;
		n--;
	}
	return (s);
}

void	*ft_memset2(void *s, int c, size_t n)
{
	unsigned short	buf;
	size_t			i;

	i = sizeof(buf);
	buf = 0;
	while (i > 0)
	{
		buf <<= 8;
		buf += (unsigned char)c;
		--i;
	}
	while (n > 0)
	{
		((unsigned short *)s)[n - 1] = buf;
		n -= sizeof(buf);
	}
	return (s);
}

void	*ft_memset4(void *s, int c, size_t n)
{
	unsigned int	buf;
	size_t			i;

	buf = 0;
	i = sizeof(buf);
	while (i > 0)
	{
		buf <<= 8;
		buf += (unsigned char)c;
		--i;
	}
	while (n > 0)
	{
		((unsigned int *)s)[n - 1] = buf;
		n -= sizeof(buf);
	}
	return (s);
}

void	*ft_memset8(void *s, int c, size_t n)
{
	unsigned long	buf;
	size_t			i;

	buf = 0;
	i = sizeof(buf);
	while (i > 0)
	{
		buf <<= 8;
		buf += (unsigned char)c;
		--i;
	}
	while (n > 0)
	{
		((unsigned long *)s)[n - 1] = buf;
		n -= sizeof(buf);
	}
	return (s);
}

void	*ft_memset(void *s, int c, size_t n)
{
	if (n % 8 == 0)
		return (ft_memset8(s, c, n));
	else if (n % 4 == 0)
		return (ft_memset4(s, c, n));
	else if (n % 2 == 0)
		return (ft_memset2(s, c, n));
	return (ft_memset1(s, c, n));
}
