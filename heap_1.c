/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edsole-a <edsole-a@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 10:57:30 by edsole-a          #+#    #+#             */
/*   Updated: 2026/10/02 12:51:03 by edsole-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool    heap_init(t_heap *h, int capacity)
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
	return (true);
}

int	find_min_index(t_heap *heap)
{
	int	i;
	int	min_i;

	min_i = 0;
	i = 1;
	while (i < heap->size)
	{
		if (heap->data[i].priority < heap->data[min_i].priority)
			min_i = i;
		i++;
	}
	return (min_i);
}