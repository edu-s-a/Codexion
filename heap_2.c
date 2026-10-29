/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 12:15:39 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/29 12:20:24 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	heap_less(t_request *a, t_request *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (a->coder_id < b->coder_id);
}

static void	heap_swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_sift_up(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!heap_less(&heap->data[i], &heap->data[parent]))
			return ;
		heap_swap(&heap->data[i], &heap->data[parent]);
		i = parent;
	}
}

void	heap_sift_down(t_heap *heap, int i)
{
	int	smallest;
	int	child;

	while (true)
	{
		smallest = i;
		child = 2 * i + 1;
		if (child < heap->size
			&& heap_less(&heap->data[child], &heap->data[smallest]))
			smallest = child;
		child++;
		if (child < heap->size
			&& heap_less(&heap->data[child], &heap->data[smallest]))
			smallest = child;
		if (smallest == i)
			return ;
		heap_swap(&heap->data[i], &heap->data[smallest]);
		i = smallest;
	}
}

bool	heap_remove(t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->data[i].coder_id != coder_id)
		i++;
	if (i == heap->size)
		return (false);
	heap->size--;
	if (i < heap->size)
	{
		heap->data[i] = heap->data[heap->size];
		heap_sift_down(heap, i);
		heap_sift_up(heap, i);
	}
	return (true);
}
