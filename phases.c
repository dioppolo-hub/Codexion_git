/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phases.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:18:12 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/21 11:19:23 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	get_phase_size(t_env *env, int phase)
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
	int	my_phase;

	if (coder->init_acq_done)
		return ;
	my_phase = get_coder_phase(coder);
	pthread_mutex_lock(&coder->env->start_mutex);
	while (is_simulation_running(coder->env)
		&& coder->env->initial_phase < my_phase)
		pthread_cond_wait
		(
			&coder->env->start_cond,
			&coder->env->start_mutex
		);
	pthread_mutex_unlock(&coder->env->start_mutex);
}

void	complete_initial_phase(t_coder *coder)
{
	int	my_phase;

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
		pthread_cond_wait
		(
			&coder->env->start_cond,
			&coder->env->start_mutex
		);
	pthread_mutex_unlock(&coder->env->start_mutex);
}
