#include "../inc/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

void	ft_malloc(size_t size)
{
	t_block	*region;
	t_block	*block;
	t_block	*block2;

	if ((region = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		ft_putstr("mmap failed !\n");
	g_begin.tiny = region;
	block = (t_block *)region->data;
	block->size = 42;
	block->used = 1;
	block->next = (t_block *)(block->data + block->size);
	block2 = block->next;
	block2->size = 43;
	block2->used = 1;
	block2->next = NULL;
}