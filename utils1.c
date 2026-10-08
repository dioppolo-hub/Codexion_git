/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 14:02:03 by diego             #+#    #+#             */
/*   Updated: 2026/10/08 16:26:21 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//inserisce la richiesta nell'heap
//attende finché non è il primo e il cooldown non è scaduto
//se è il suo turno in cima
//se il cooldown è finito break
//se non è il primo in coda si mette in attesa
static void	wait_pair_cooldown(t_env *env, long long remaining)
{
	long long		wake_time;
	struct timespec	ts;

	wake_time = get_time_ms() + remaining;
	ts.tv_sec = wake_time / 1000;
	ts.tv_nsec = (wake_time % 1000) * 1000000;
	pthread_cond_timedwait(&env->pair_cond, &env->pair_mutex, &ts);
}

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


void	lock_both_dongles(t_coder *coder)
{
	t_env		*env;
	t_dongle	*left;
	t_dongle	*right;
	t_request	req;
	t_request	top;
	long long	ready_at;
	long long	remaining;

	env = coder->env;
	left = coder->left_dongle;
	right = coder->right_dongle;
	req.coder_id = coder->id;
	req.request_time = get_time_ms() - env->start_time;
	pthread_mutex_lock(&env->sim_mutex);
	req.deadline = coder->deadline;
	pthread_mutex_unlock(&env->sim_mutex);
	pthread_mutex_lock(&env->pair_mutex);
	heap_push(&env->pair_heap, req, env->scheduler_type);
	while(is_simulation_running(env))
	{
		top = heap_peek(&env->pair_heap);
		ready_at = left->available_at;
		if (right->available_at > ready_at)
			ready_at = right->available_at;
		remaining = ready_at - get_time_ms();
		if (top.coder_id == coder->id
			&& !left->is_in_use && !right->is_in_use
			&& remaining <= 0)
		{
			heap_pop(&env->pair_heap, env->scheduler_type);
			left->is_in_use = true;
			right->is_in_use = true;
			pthread_cond_broadcast(&env->pair_cond);
			break ;
		}
		if (top.coder_id == coder->id && remaining > 0
			&& !left->is_in_use && !right->is_in_use)
			wait_pair_cooldown(env, remaining);
		else
			pthread_cond_wait(&env->pair_cond, &env->pair_mutex);
	}
	if (!is_simulation_running(env))
		heap_remove(&env->pair_heap, coder->id, env->scheduler_type);
	pthread_mutex_unlock(&env->pair_mutex);
	if (is_simulation_running(env))
		log_both_dongles(coder);
}
