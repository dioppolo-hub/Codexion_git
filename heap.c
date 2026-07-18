/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:55:55 by diego             #+#    #+#             */
/*   Updated: 2026/07/18 15:27:13 by diego            ###   ########.fr       */
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
	int i;
	int parent;
	t_request tmp;

	if (heap->size >= heap->capacity)
		return; //struttura piena
	//inserisco il nuovo elemento alla fine dell'array
	i = heap->size;
	heap->request[i] = req;
	heap->size++;
	//sposta l'elemento verso l'alto finché ha priorità
	while (i > 0)
	{
		parent = (i - 1) / 2;
		//se la richiesta ha più priorità del padre, scambiale
		if (compare_requests(heap->request[i], heap->request[parent], scheduler_type))
		{
			tmp = heap->request[i];
			heap->request[i] = heap->request[parent];
			heap->request[parent] = tmp;
			i = parent; //mi salvo l'indice del padre
		}
		else
			break;
	}
}

//rimuove e restituisce la richiesta in cima
t_request heap_pop(t_heap *heap, int scheduler_type)
{
	t_request top;
	int i;
	int left;
	int right;
	int smallest;

	top = heap->request[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->request[0] = heap->request[heap->size];
		i = 0;
		while (1)
		{
			left = 2 * i + 1;
			right = 2 * i + 2;
			smallest = i;
			if (left < heap->size && compare_requests(heap->request[left], heap->request[smallest], scheduler_type))
				smallest = left 
		}
	}
}

//ritorna la richiesta in cima senza rimuoverla
t_request heap_peek(t_heap *heap)
{
	return (heap->request[0]);
}