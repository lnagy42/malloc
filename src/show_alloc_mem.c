#include "../inc/malloc.h"

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

#include <unistd.h>

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

size_t		ft_size(size_t size, unsigned int base)
{
	size_t	power;

	power = 1;
	while (size /= base)
		power *= base;
	return (power);
}

void	ft_putsize_base(size_t size, unsigned int base)
{
	char	buff[66];
	static const char	table[16] = "0123456789ABCDEF";
	size_t	power;
	int		i;

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

void	ft_putaddr(void *addr)
{
	ft_putsize_base((size_t)addr, 16);
}

void	put_region(t_block *beginlist_region, char *area, size_t *total)
{
	t_block	*block;

	while (beginlist_region)
	{
		ft_putstr(area);
		ft_putaddr(beginlist_region->data);
		ft_putstr("\n");
		block = (t_block *)beginlist_region->data;
		while (block)
		{
			if (block->used)
			{
				ft_putaddr(block->data);
				ft_putstr(" - ");
				ft_putaddr(block->data + block->size);
				ft_putstr(" : ");
				ft_putsize_base(block->size, 10);
				ft_putstr(" octets\n");
				*total += block->size;
			}
			block = block->next;	
		}
		beginlist_region = beginlist_region->next;
	}
}

void	show_alloc_mem(void)
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
