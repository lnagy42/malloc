#include "../inc/malloc.h"
#include <unistd.h>

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
		put_size_dbg(beginlist_region->size);
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
			block = block->next;	
		}
		beginlist_region = beginlist_region->next;
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

void	mutex_error(char *s, int ret)
{
	ft_putstr_fd(s, 2);
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