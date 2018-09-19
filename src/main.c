#include "../inc/malloc.h"

int main(void)
{
	void	*ptr;
	void	*ptr2;
	void	*ptr3;
	// void	*ptr4;

	// ptr = ft_malloc(512);
	// ptr2 = ft_malloc(350);
	// ptr3 = ft_malloc(440);
	// show_alloc_mem();
	// ft_free(ptr);
	// ft_free(ptr2);
	// ft_free(ptr3);
	// show_alloc_mem();
	ptr = malloc(3312);
	// show_alloc_mem();
	ptr2 = malloc(65536);
	show_alloc_mem();
	ptr = realloc(ptr, 1803292);
	show_alloc_mem();
	// ptr2 = ft_malloc(465);
	// ptr = ft_realloc(ptr, 58);
	// show_alloc_mem();
	// ptr2 = ft_malloc(215);
	// ft_free(ptr);
	// ft_free(ptr2);
	// ft_free(ptr3);
	// show_alloc_mem();
	// ptr = ft_malloc(570);
	// ptr2 = ft_malloc(115);
	// show_alloc_mem();
	// ptr2 = ft_realloc(ptr2, 128);
	// show_alloc_mem();
	// ft_malloc(1000);
	// ft_free(ptr2);
	// show_alloc_mem();
	// ft_malloc(200);
	// show_alloc_mem();
	// ft_malloc(12000);
	// ft_malloc(3000);
	// ft_malloc(18990);
	// ft_malloc(20000);
	return(0);
}