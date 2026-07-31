/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:02:03 by diego             #+#    #+#             */
/*   Updated: 2026/07/31 14:55:56 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request req;
	t_request top;
	long long curr_time;
	long long t_remaining;
	struct timespec ts;
    long long wake_time;

	pthread_mutex_lock(&dongle->mutex);
	//inserisce la richiesta nell'heap
	req.coder_id = coder->id;
	req.request_time = get_time_ms() - coder->env->start_time;
	pthread_mutex_lock(&coder->env->sim_mutex);
	req.deadline = coder->last_compile_start + coder->env->t_burnout;
	pthread_mutex_unlock(&coder->env->sim_mutex);
	heap_push(&dongle->heap, req, coder->env->scheduler_type);
	//attende finché non è il primo e il cooldown non è scaduto
	pthread_cond_broadcast(&dongle->cond);
	while (is_simulation_running(coder->env))
	{
		top = heap_peek(&dongle->heap);
		if (top.coder_id == coder->id) //se è il suo turno in cima
		{
			curr_time = get_time_ms() - coder->env->start_time;
			t_remaining = (dongle->last_released_time + coder->env->cooldown) - curr_time;
			if (t_remaining <= 0) //se il cooldown è finito break
				break;
			wake_time = get_time_ms() + t_remaining;
            ts.tv_sec = wake_time / 1000;
            ts.tv_nsec = (wake_time % 1000) * 1000000;
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		//se non è il primo in coda si mette in attesa
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	//rimuove la richiesta dall'heap
	if (is_simulation_running(coder->env))
        heap_pop(&dongle->heap, coder->env->scheduler_type);
	pthread_mutex_unlock(&dongle->mutex);
}

void release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	//aggiorna l'orario di ultimo rilascio
	dongle->last_released_time = get_time_ms() - coder->env->start_time;
	//sveglia gli altri coder in attesa su questo dongle
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void lock_both_dongles(t_coder *coder)
{
	t_dongle *first;
	t_dongle *second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	//per prevenire stalli ordina in base all'ID
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	acquire_dongle(coder, first);
	acquire_dongle(coder, second);
}

void release_both_dongle(t_coder *coder)
{
	release_dongle(coder, coder->left_dongle);
	release_dongle(coder, coder->right_dongle);
}

long long get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}
