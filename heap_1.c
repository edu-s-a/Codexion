/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 10:57:30 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/27 12:46:31 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#include "codexion.h"

bool	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_request) * capacity);
	if (!h->data)
		return (false);
	h->capacity = capacity;
	h->size = 0;
	return (true);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->data);
	heap->data = NULL;
	heap->size = 0;
	heap->capacity = 0;
}

bool	heap_push(t_heap *heap, int coder_id, long priority)
{
	if (heap->size >= heap->capacity)
		return (false);
	heap->data[heap->size].coder_id = coder_id;
	heap->data[heap->size].priority = priority;
	heap->size++;
	heap_sift_up(heap, heap->size - 1);
	return (true);
}

bool	heap_peek(t_heap *heap, t_request *out)
{
	if (heap->size == 0)
		return (false);
	*out = heap->data[0];
	return (true);
}

bool	heap_pop(t_heap *heap, t_request *out)
{
	if (heap->size == 0)
		return (false);
	*out = heap->data[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->data[0] = heap->data[heap->size];
		heap_sift_down(heap, 0);
	}
	return (true);
}
