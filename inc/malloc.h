/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnagy <lnagy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 17:52:26 by lnagy             #+#    #+#             */
/*   Updated: 2018/09/26 18:08:10 by lnagy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <stddef.h>
# include <sys/mman.h>
# include <pthread.h>
# include <stdint.h>

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
pthread_mutex_t		g_mutex;

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t count, size_t size);
void	show_alloc_mem(void);
void	*ft_malloc_thread_unsafe(size_t size);
void	*ft_realloc_thread_unsafe(void *ptr, size_t size);
void	ft_free_thread_unsafe(void *ptr);
void	mutex_error(char *s, int ret);
void	ft_putstr(char *s);
void	ft_putstr_fd(char *s, int ret);
void	ft_putsize_base(size_t size, unsigned int base);
void	ft_putaddr(void *addr);
t_zone	find_block(void *ptr,t_zone *prev);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
size_t	align(size_t size, size_t align);
t_block	**choose_list(size_t size, size_t *region_size);
t_block	*check_prevblock_pushback(size_t size, t_block *new_block,
	t_block *region, t_zone *prev);
t_block	*init_region(t_block **region, size_t size, size_t region_size);
void	ft_putstr_fd(char *s, int fd);
int		ft_strlen(char *str);
void	ft_putstr(char *s);
size_t	ft_size(size_t size, unsigned int base);
void	ft_putaddr(void *addr);

void	put_request_malloc_dbg(size_t size);
void	put_request_free_dbg(void *addr);
void	put_request_realloc_dbg(void *addr, size_t size);
void	*put_ret_addr_dbg(void *addr);
void	put_size_dbg(size_t size);

#endif

