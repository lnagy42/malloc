#include "../inc/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

size_t		align(size_t size, size_t align)
{
	size_t	shift;
	size_t	temp;

	if (size <= align)
		return (align);
	temp = align;
	shift = 0;
	while (temp >= 2)
	{
		shift++;
		temp >>= 1;
	}
	/* decalage binaire droite ">>" == "diviser par multiple de 2". shift == calculer l'alignement (1 en binaire == /2)*/
	return ((((size - 1) >> shift) << shift) + align);
}

t_block		**choose_list(size_t size, size_t *region_size)
{
	t_block	**begin_list;
	t_block	*begin_region;

	if (size <= 2048)
	{
		begin_list = &g_begin.tiny;
		*region_size = align(2048 * 100, getpagesize());
	}
	else if (size > 2048 && size <= 16384)
	{
		begin_list = &g_begin.small;
		*region_size = align(16384 * 100, getpagesize());
	}
	else
	{
		begin_list = &g_begin.large;
		*region_size = align(size, getpagesize());
	}
	return (begin_list);
}

t_block		*init_first_region(t_block **region, size_t size, size_t region_size)
{
	t_block	*init_region;
	t_block	*init_block;

	if (!*region)
	{
		if ((init_region = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		{
			ft_putstr("MAP FAILED !\n");
			return (NULL);
		}
		init_region->size = region_size;
		init_region->used = 1;
		init_region->next = NULL;
		init_block = (t_block *)init_region->data;
		init_block->size = size;
		init_block->used = 1;
		init_block->next = NULL;
		*region = init_region;
		return (init_block);
	}
	return (NULL);
}

t_block	*create_block(size_t size, t_block *first_block)
{
	t_block	*new_block;

	new_block = (t_block *)(first_block->data + first_block->size);
	first_block->next = new_block;
	new_block->used = 1;
	new_block->size = size;
	new_block->next = NULL;
	return (new_block);
}

void	ft_malloc(size_t size)
{
	t_block	**list;
	t_block	*block;
	size_t	region_size;

	size = align(size, PADDING);
	list = choose_list(size, &region_size);
	if (!(block = init_first_region(list, size, region_size)))
		block = create_block(size, (t_block *)(*list)->data);
}

int main(void)
{
	ft_malloc(56);
	ft_malloc(60);
	ft_malloc(4556);
	ft_malloc(18990);
	show_alloc_mem();
	return(0);
}