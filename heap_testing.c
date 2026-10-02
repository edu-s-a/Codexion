#include "codexion.h"

static void	test_basic_pop_order(void)
{
	t_heap		heap;
	t_request	out;

	printf("=== test 1: push scrambled, pop ascending ===\n");
	heap_init(&heap, 10);
	heap_push(&heap, 0, 50);
	heap_push(&heap, 1, 10);
	heap_push(&heap, 2, 30);
	heap_push(&heap, 3, 5);
	heap_push(&heap, 4, 40);
	while (!heap_is_empty(&heap))
	{
		heap_pop(&heap, &out);
		printf("coder_id=%d priority=%ld\n", out.coder_id, out.priority);
	}
	heap_destroy(&heap);
}

static void	test_remove_middle(void)
{
	t_heap		heap;
	t_request	out;
	bool		removed;

	printf("=== test 2: push, remove middle, pop rest ===\n");
	heap_init(&heap, 10);
	heap_push(&heap, 10, 100);
	heap_push(&heap, 11, 20);
	heap_push(&heap, 12, 60);
	heap_push(&heap, 13, 10);
	heap_push(&heap, 14, 80);
	removed = heap_remove(&heap, 12);
	printf("removed coder 12: %s\n", removed ? "true" : "false");
	while (!heap_is_empty(&heap))
	{
		heap_pop(&heap, &out);
		printf("coder_id=%d priority=%ld\n", out.coder_id, out.priority);
	}
	heap_destroy(&heap);
}

int	main(void)
{
	test_basic_pop_order();
	test_remove_middle();
	return (0);
}