/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:19:48 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/15 10:20:16 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//inserisce una nuova richiesta nell'heap
void	heap_push(t_heap *heap, t_request req, int scheduler_type)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->request[heap->size] = req;
	heap->size++;
	heapify_up(heap, heap->size - 1, scheduler_type);
}

//rimuove e restituisce la richiesta in cima
t_request	heap_pop(t_heap *heap, int scheduler_type)
{
	t_request	top;

	top = heap->request[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->request[0] = heap->request[heap->size];
		heapify_down(heap, 0, scheduler_type);
	}
	return (top);
}

//ritorna la richiesta in cima senza rimuoverla
t_request	heap_peek(t_heap *heap)
{
	return (heap->request[0]);
}

void	heap_clear(t_heap *heap)
{
	if (heap->request)
		free(heap->request);
	heap->request = NULL;
	heap->size = 0;
	heap->capacity = 0;
}
