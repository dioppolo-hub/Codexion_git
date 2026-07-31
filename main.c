/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:08:27 by dioppolo          #+#    #+#             */
/*   Updated: 2026/07/31 14:41:16 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool init_dongles(t_env *env)
{
	int i;

	env->dongles = malloc(sizeof(t_dongle) * env->num_coders);
	if (!env->dongles)
		return false;
	i = 0;
	while (i < env->num_coders)
	{
		env->dongles[i].id = i;
		env->dongles[i].last_released_time = 0;
		if (pthread_mutex_init(&env->dongles[i].mutex, NULL) != 0)
			return false;
		if (pthread_cond_init(&env->dongles[i].cond, NULL) != 0)
			return false;
		heap_init(&env->dongles[i].heap, env->num_coders);
		i++;
	}
	return true;
}

t_coder *init_coders(t_env *env)
{
	t_coder *coders;
	int i;

	coders = malloc(sizeof(t_coder) * env->num_coders);
	if (!coders)
		return NULL;
	i = 0;
	while (i < env->num_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_start = env->start_time;
		coders[i].env = env;
		coders[i].left_dongle = &env->dongles[i];
		coders[i].right_dongle = &env->dongles[(i + 1) % env->num_coders];
	}
	return coders;
}

void cleanup(t_env *env, t_coder *coders)
{
	int i;

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

bool start_simulation(t_env *env, t_coder *coders)
{
	pthread_t *threads;
	pthread_t monitor;
	int i;

	threads = malloc(sizeof(pthread_t) * env->num_coders);
	if (!threads)
		return false;
	env->start_time = get_time_ms();
	i = 0;
	while (i < env->num_coders)
	{
		coders[i].last_compile_start = env->start_time;
		if (pthread_create(&threads[i], NULL, coder_routine, &coders[i]) != 0)
		{
			free(threads);
			return false;
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, coders) != 0)
	{
		free(threads);
		return (false);
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < env->num_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free (threads);
	return true;
}

int main(int argc, char **argv)
{
	t_env env;
	t_coder *coders;

	if (!parcing_1(argc, argv))
	{
		printf("Error: Invalid argumets\n");
		return 1;
	}
	pthread_mutex_init(&env.write_mutex, NULL);
	pthread_mutex_init(&env.sim_mutex, NULL);
	env.simulation_running = 1;
	if (!init_dongles(&env))
	{
		cleanup(&env, NULL);
		return 1;
	}
	coders = init_coders(&env);
	if (!coders)
	{
		cleanup(&env, NULL);
		return 1;
	}
	if (!start_simulation(&env, coders))
	{
		printf("Error creating threads\n");
		cleanup(&env, coders);
		return 1;
	}
	cleanup(&env, coders);
	return 0;
}
