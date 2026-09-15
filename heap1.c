/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 14:55:55 by diego             #+#    #+#             */
/*   Updated: 2026/09/15 12:21:56 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//IF = FIFO, ELSE = EDF
//se i tempi o le scadenze sono uguali vince l'ID minore
int	comp_requests(t_request a, t_request b, int scheduler_type)
{
	if (scheduler_type == 0)
	{
		if (a.request_time != b.request_time)
			return (a.request_time < b.request_time);
	}
	else
	{
		if (a.deadline != b.deadline)
			return (a.deadline < b.deadline);
	}
	return (a.coder_id < b.coder_id);
}

//Spinge un elemento verso l'alto nell'heap finché non trova la sua
//posizione corretta per rispettare la priorità
void	heapify_up(t_heap *heap, int i, int scheduler_type)
{
	int			parent;
	t_request	tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (comp_requests
			(
				heap->request[i], heap->request[parent], scheduler_type
			))
		{
			tmp = heap->request[i];
			heap->request[i] = heap->request[parent];
			heap->request[parent] = tmp;
			i = parent;
		}
		else
			break ;
	}
}

//Spinge un elemento verso il basso nell'heap confrontandolo
//con i suoi figli.
void	heapify_down(t_heap *heap, int i, int scheduler_type)
{
	int			cld;
	int			left;
	int			right;
	t_request	tmp;

	while ((2 * i + 1) < heap->size)
	{
		left = 2 * i + 1;
		right = left + 1;
		cld = left;
		if (
			right < heap->size && comp_requests(heap->request[right],
				heap->request[left], scheduler_type))
			cld = right;
		if (
			comp_requests(heap->request[i], heap->request[cld], scheduler_type))
		{
			tmp = heap->request[i];
			heap->request[i] = heap->request[cld];
			heap->request[cld] = tmp;
			i = cld;
		}
		else
			break ;
	}
}

//alloca memoria in base al numero di coders
void	heap_init(t_heap *heap, int capacity)
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
