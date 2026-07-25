/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:55:55 by diego             #+#    #+#             */
/*   Updated: 2026/07/22 16:34:49 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int compare_requests(t_request a, t_request b, int scheduler_type)
{
	if (scheduler_type == 0) //FIFO
	{
		if (a.request_time != b.request_time)
			return (a.request_time < b.request_time);
	}
	else //EDF
	{
		if (a.deadline != b.deadline)
			return (a.deadline < b.deadline);
	}
	//se i tempi o le scadenze sono uguali vince l'ID minore
	return (a.coder_id < b.coder_id);
}

//Spinge un elemento verso l'alto nell'heap finché non trova la sua
//posizione corretta per rispettare la priorità
static void heapify_up(t_heap *heap, int i, int scheduler_type)
{
	int parent;
	t_request tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (compare_requests(heap->request[i], heap->request[parent], scheduler_type))
		{
			tmp = heap->request[i];
			heap->request[i] = heap->request[parent];
			heap->request[parent] = tmp;
			i = parent;
		}
		else
			break;
	}
}

//Spinge un elemento verso il basso nell'heap confrontandolo
//con i suoi figli.
static void heapify_down(t_heap *heap, int i, int scheduler_type)
{
	int child;
	int left;
	int right;
	t_request tmp;

	while ((left = 2 * i + 1) < heap->size)
	{
		right = left + 1;
		child = left;
		if (right < heap->size && compare_requests(heap->request[right], heap->request[left], scheduler_type))
		if (compare_requests(heap->request[i], heap->request[child], scheduler_type))
		{
			tmp = heap->request[i];
			heap->request[i] = heap->request[child];
			heap->request[child] = tmp;
			i = child;
		}
		else
			break;
	}
}

//alloca memoria in base al numero di coders
void heap_init(t_heap *heap, int capacity)
{
	heap->request = malloc(sizeof(t_request) * capacity);
	if (!heap->request)
	{
		printf("Malloc heap fallita");
		exit(0);
	}
	heap->size = 0;
	heap->capacity = capacity;
}

//inserisce una nuova richiesta nell'heap
void heap_push(t_heap *heap, t_request req, int scheduler_type)
{
	if (heap->size >= heap->capacity)
		return;
	heap->request[heap->size] = req;
	heap->size++;
	heapify_up(heap, heap->size - 1, scheduler_type);
}

//rimuove e restituisce la richiesta in cima
t_request heap_pop(t_heap *heap, int scheduler_type)
{
	t_request top;

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
t_request heap_peek(t_heap *heap)
{
	return (heap->request[0]);
}

void heap_clear(t_heap *heap)
{
	if (heap->request)
		free(heap->request);
	heap->request = NULL;
	heap->size = 0;
	heap->capacity = 0;
}
