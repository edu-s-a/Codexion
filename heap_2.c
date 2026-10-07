/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 12:15:39 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/07 12:45:52 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	heap_peek(t_heap *heap, t_request *out)
{
	if (heap->size == 0)
		return (false);
	*out = heap->data[find_min_index(heap)];
	return (true);
}

bool	heap_pop(t_heap *heap, t_request *out)
{
	int	min_i;

	if (heap->size == 0)
		return (false);
	min_i = find_min_index(heap);
	*out = heap->data[min_i];
	heap->size--;
	heap->data[min_i] = heap->data[heap->size];
	return (true);
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
	heap->data[i] = heap->data[heap->size];
	return (true);
}

bool	heap_is_empty(t_heap *heap)
{
	return (heap->size == 0);
}
