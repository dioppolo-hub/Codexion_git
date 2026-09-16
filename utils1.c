/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:02:03 by diego             #+#    #+#             */
/*   Updated: 2026/09/16 12:36:39 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//inserisce la richiesta nell'heap
//attende finché non è il primo e il cooldown non è scaduto
//se è il suo turno in cima
//se il cooldown è finito break
//se non è il primo in coda si mette in attesa
void	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request		req;

	pthread_mutex_lock(&dongle->mutex);
	req.coder_id = coder->id;
	req.request_time = get_time_ms() - coder->env->start_time;
	pthread_mutex_lock(&coder->env->sim_mutex);
	req.deadline = coder->last_compile_start + coder->env->t_burnout;
	pthread_mutex_unlock(&coder->env->sim_mutex);
	heap_push(&dongle->heap, req, coder->env->scheduler_type);
	norm_acquire_dongle(coder, dongle);
	pthread_mutex_unlock(&dongle->mutex);
}

void	ft_wait_cooldown(t_dongle *dongle, long long t_remaining)
{
	long long		wake_time;
	struct timespec	ts;

	wake_time = get_time_ms() + t_remaining;
	ts.tv_sec = wake_time / 1000;
	ts.tv_nsec = (wake_time % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

void	norm_acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request		top;
	long long	timestamp;

	while (is_simulation_running(coder->env))
	{
		top = heap_peek(&dongle->heap);
		if (top.coder_id == coder->id && !dongle->is_in_use)
		{
			dongle->is_in_use = true;
			break ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	timestamp = get_time_ms() - coder->env->start_time;
	if (dongle == coder->right_dongle)
		printf("%lld %d has taken the right dongle\n", timestamp, coder->id);
	else
		printf("%lld %d has taken the left dongle\n", timestamp, coder->id);
}
