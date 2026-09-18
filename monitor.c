/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 14:28:55 by diego             #+#    #+#             */
/*   Updated: 2026/09/18 10:21:56 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//interrompe la sim per tutti i thread
//stampa messaggio di burnout
bool	check_coder_burnout(t_coder *coder)
{
	long long	now;
	long long	t_since_last_compile;

	pthread_mutex_lock(&coder->env->sim_mutex);
	now = get_time_ms();
	t_since_last_compile = now - coder->last_compile_start;
	pthread_mutex_unlock(&coder->env->sim_mutex);
	if (t_since_last_compile > coder->env->t_burnout)
	{
		stop_simulation(coder->env);
		pthread_mutex_lock(&coder->env->write_mutex);
		printf(
			"%lld %d died of burnout\n",
			now - coder->env->start_time, coder->id
			);
		pthread_mutex_unlock(&coder->env->write_mutex);
		return (true);
	}
	return (false);
}

//se finiscono le compilazioni ferma tutto
bool	check_all_finished(t_coder *coders, t_env *env)
{
	int	i;
	int	finished_count;

	if (env->req_compiles <= 0)
		return (false);
	finished_count = 0;
	i = 0;
	while (i < env->num_coders)
	{
		pthread_mutex_lock(&env->sim_mutex);
		if (coders[i].compile_count >= env->req_compiles)
			finished_count++;
		pthread_mutex_unlock(&env->sim_mutex);
		i++;
	}
	if (finished_count == env->num_coders)
	{
		stop_simulation(env);
		return (true);
	}
	return (false);
}

//scansiona ogni coder per check burnout
//verifica fine simulazione
//micro sleep per alleggerire la CPU
void	*monitor_routine(void *arg)
{
	t_coder	*coders;
	t_env	*env;
	int		i;

	coders = (t_coder *)arg;
	env = coders[0].env;
	while (is_simulation_running(env))
	{
		i = 0;
		while (i < env->num_coders && is_simulation_running(env))
		{
			if (check_coder_burnout(&coders[i]))
				return (NULL);
			i++;
		}
		if (check_all_finished(coders, env))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

void	stop_simulation(t_env *env)
{
	int	i;

	pthread_mutex_lock(&env->sim_mutex);
	env->simulation_running = 0;
	pthread_mutex_unlock(&env->sim_mutex);

	i = 0;
	while (i < env->num_coders)
	{
		pthread_mutex_lock(&env->dongles[i].mutex);
		pthread_cond_broadcast(&env->dongles[i].cond);
		pthread_mutex_unlock(&env->dongles[i].mutex);
		i++;
	}
}
