/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:08:27 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/15 12:25:54 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_env *env, t_coder *coders)
{
	int	i;

	if (env->dongles)
	{
		i = 0;
		while (i < env->num_coders)
		{
			pthread_mutex_destroy(&env->dongles[i].mutex);
			pthread_cond_destroy(&env->dongles[i].cond);
			heap_clear(&env->dongles[i].heap);
			i++;
		}
		free(env->dongles);
	}
	pthread_mutex_destroy(&env->write_mutex);
	pthread_mutex_destroy(&env->sim_mutex);
	if (coders)
		free(coders);
}

bool	start_simulation(t_env *env, t_coder *coders)
{
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	threads = malloc(sizeof(pthread_t) * env->num_coders);
	if (!threads)
		return (false);
	if (!norm_start_sim(env, threads, coders))
	{
		free(threads);
		return (false);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, coders) != 0)
	{
		free(threads);
		return (false);
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < env->num_coders)
		pthread_join(threads[i++], NULL);
	free (threads);
	return (true);
}

bool	norm_start_sim(t_env *env, pthread_t *threads, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < env->num_coders)
	{
		coders[i].last_compile_start = env->start_time;
		if (pthread_create(&threads[i], NULL, coder_routine, &coders[i]) != 0)
		{
			return (false);
		}
		i++;
	}
	return (true);
}

static int	init_and_setup(int argc, char **argv, t_env *env, t_coder **coders)
{
	if (!parcing_1(argc, argv, env))
	{
		printf("Error: Invalid argumets\n");
		return (0);
	}
	pthread_mutex_init(&env->write_mutex, NULL);
	pthread_mutex_init(&env->sim_mutex, NULL);
	env->simulation_running = 1;
	env->start_time = get_time_ms();
	if (!init_dongles(env))
		return (0);
	*coders = init_coders(env);
	if (!*coders)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_env	env;
	t_coder	*coders;

	if (!init_and_setup(argc, argv, &env, &coders))
		return (1);
	if (!start_simulation(&env, coders))
	{
		printf("Error creating threads\n");
		cleanup(&env, coders);
		return (1);
	}
	cleanup(&env, coders);
	return (0);
}
