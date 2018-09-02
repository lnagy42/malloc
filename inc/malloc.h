#ifndef MALLOC_H
# define MALLOC_H
# include <stddef.h>
# define PADDING 8

typedef struct		s_block
{
	size_t			size;
	int				used;
	struct s_block	*next;
	char			data[0];
}					t_block;

typedef struct		s_region
{
	t_block			*tiny;
	t_block			*small;
	t_block			*large;
}					t_region;

t_region			g_begin;

void	ft_malloc(size_t size);
void	show_alloc_mem(void);
void	ft_putstr(char *s);

#endif

