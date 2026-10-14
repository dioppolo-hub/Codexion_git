/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:02:03 by diego             #+#    #+#             */
/*   Updated: 2026/10/14 10:35:28 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//inserisce la richiesta nell'heap
//attende finché non è il primo e il cooldown non è scaduto
//se è il suo turno in cima
//se il cooldown è finito break
//se non è il primo in coda si mette in attesa

static void	log_both_dongles(t_coder *coder)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->env->write_mutex);
	pthread_mutex_lock(&coder->env->sim_mutex);
	if (coder->env->simulation_running)
	{
		timestamp = get_time_ms() - coder->env->start_time;
		printf("%lld %d has taken the left dongle\n", timestamp, coder->id);
		printf("%lld %d has taken the right dongle\n", timestamp, coder->id);
	}
	pthread_mutex_unlock(&coder->env->sim_mutex);
	pthread_mutex_unlock(&coder->env->write_mutex);
}

static t_request	make_pair_request(t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	req.request_time = get_time_ms() - coder->env->start_time;
	pthread_mutex_lock(&coder->env->sim_mutex);
	req.deadline = coder->deadline;
	pthread_mutex_unlock(&coder->env->sim_mutex);
	return (req);
}

static long long	get_pair_ready_at(t_coder *coder)
{
	if (coder->right_dongle->available_at
		> coder->left_dongle->available_at)
		return (coder->right_dongle->available_at);
	return (coder->left_dongle->available_at);
}

bool	pair_is_free(t_coder *coder)
{
	return (!coder->left_dongle->is_in_use
		&& !coder->right_dongle->is_in_use);
}

void	lock_both_dongles(t_coder *coder, long long rdy_at, long long rem)
{
	t_env		*env;
	t_request	req;
	t_request	top;

	env = coder->env;
	req = make_pair_request(coder);
	pthread_mutex_lock(&env->pair_mutex);
	heap_push(&env->pair_heap, req, env->scheduler_type);
	while (is_simulation_running(env))
	{
		top = heap_peek(&env->pair_heap);
		rdy_at = get_pair_ready_at(coder);
		rem = rdy_at - get_time_ms();
		if (is_pair_turn(coder, top) && rem <= 0)
		{
			grant_pair(coder);
			break ;
		}
		check_pair_turn(coder, rem, top);
	}
	if (!is_simulation_running(env))
		heap_remove(&env->pair_heap, coder->id, env->scheduler_type);
	pthread_mutex_unlock(&env->pair_mutex);
	if (is_simulation_running(env))
		log_both_dongles(coder);
}
