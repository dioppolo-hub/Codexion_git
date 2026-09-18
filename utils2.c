/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:30:39 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/18 12:15:24 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//aggiorna l'orario di ultimo rilascio
//sveglia gli altri coder in attesa su questo dongle
void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	bool	running;

	running = is_simulation_running(coder->env);
	pthread_mutex_lock(&dongle->mutex);
	if (running)
		heap_pop(&dongle->heap, coder->env->scheduler_type);
	dongle->is_in_use = false;
	dongle->last_released_time = get_time_ms() - coder->env->start_time;
	if (coder->env->cooldown > 0)
		ft_wait_cooldown(dongle, coder->env->cooldown);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	lock_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	acquire_dongle(coder, first);
	acquire_dongle(coder, second);
}

int	get_coder_phase(t_coder *coder)
{
	int	n;

	n = coder->env->num_coders;
	if (n == 1)
		return (0);
	if (n % 2 == 0)
	{
		if (coder->id % 2 == 1)
			return (0);
		return (1);
	}
	if (coder->id < n && coder->id % 2 == 1)
		return (0);
	if (coder->id < n)
		return (1);
	return (2);
}

int get_phase_size(t_env *env, int phase)
{
	if (env->num_coders == 1)
		return (1);
	if (env->num_coders % 2 == 0)
		return (env->num_coders / 2);
	if (phase < 2)
		return (env->num_coders / 2);
	return (1);
}

void	wait_initial_phase(t_coder *coder)
{
	int my_phase;

	if (coder->init_acq_done)
		return ;
	my_phase = get_coder_phase(coder);
	pthread_mutex_lock(&coder->env->start_mutex);
	while (is_simulation_running(coder->env)
		&& coder->env->initial_phase < my_phase)
		pthread_cond_wait(
			&coder->env->start_cond,
			&coder->env->start_mutex
		);
	pthread_mutex_unlock(&coder->env->start_mutex);
}

void	complete_initial_phase(t_coder *coder)
{
	int my_phase;

	if (coder->init_acq_done)
		return ;
	my_phase = get_coder_phase(coder);
	coder->init_acq_done = true;
	pthread_mutex_lock(&coder->env->start_mutex);
	coder->env->phase_completed++;
	if (coder->env->phase_completed
		== get_phase_size(coder->env, my_phase))
	{
		coder->env->phase_completed = 0;
		coder->env->initial_phase++;
		pthread_cond_broadcast(&coder->env->start_cond);
	}
	while (is_simulation_running(coder->env)
		&& coder->env->initial_phase == my_phase)
		pthread_cond_wait(
			&coder->env->start_cond,
			&coder->env->start_mutex
		);
	pthread_mutex_unlock(&coder->env->start_mutex);
}

void	release_both_dongle(t_coder *coder)
{
	release_dongle(coder, coder->left_dongle);
	release_dongle(coder, coder->right_dongle);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}
