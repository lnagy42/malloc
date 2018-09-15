#ifndef MALLOC_H
# define MALLOC_H
# include <stddef.h>
# include <sys/mman.h>

# ifndef DEBUG
#  define DEBUG 0
# endif

# ifndef PADDING
#  define PADDING 8
# endif

typedef struct		s_block
{
	size_t			size;
	size_t			max_size;
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

typedef struct		s_zone
{
	t_block			*region;
	t_block			*block;
	t_block			**begin_list;
}					t_zone;

t_region			g_begin;

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);
void	*ft_realloc(void *ptr, size_t size);
void	show_alloc_mem(void);
void	ft_putstr(char *s);
void	ft_putsize_base(size_t size, unsigned int base);
void	ft_putaddr(void *addr);
t_zone	find_block(void *ptr,t_zone *prev);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	align(size_t size, size_t align);


void	put_request_malloc_dbg(size_t size);
void	put_request_free_dbg(void *addr);
void	put_request_realloc_dbg(void *addr, size_t size);
void	*put_ret_addr_dbg(void *addr);
void	put_size_dbg(size_t size);

#endif

