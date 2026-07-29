/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 17:16:42 by diego             #+#    #+#             */
/*   Updated: 2026/07/29 19:28:06 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print_status(t_coder *coder, char *status)
{
	long long timestamp;

	pthread_mutex_lock(&coder->env->write_mutex);
	pthread_mutex_lock(&coder->env->sim_mutex);
	//print solo se la simulzione è ancora attiva
	if(coder->env->simulation_running)
	{
		timestamp = get_time_ms() - coder->env->start_time;
		printf("%lld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->env->sim_mutex);
	pthread_mutex_unlock(&coder->env->write_mutex);
}

bool is_simulation_running(t_env *env)
{
	bool running;

	pthread_mutex_lock(&env->sim_mutex);
	running = env->simulation_running;
	pthread_mutex_unlock(&env->sim_mutex);
	return (running);
}

//sleep per rispondere alla fine della simulazione
void smart_sleep(long long time_in_ms, t_env *env)
{
	long long start;

	start = get_time_ms();
	while (is_simulation_running(env))
	{
		if (get_time_ms() - start >= time_in_ms)
			break;
		usleep(500);
	}
}

static void coder_debug(t_coder *coder)
{
	print_status(coder, "is debugging");
	smart_sleep(coder->env->t_debug, coder->env);
}

static void coder_compile(t_coder *coder)
{
	lock_both_dongles(coder);
	pthread_mutex_lock(&coder->env->sim_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->env->sim_mutex);
	print_status(coder, "is compiling");
	smart_sleep(coder->env->t_compile, coder->env);
	coder->compile_count++;
	release_both_dongle(coder);
}

static void coder_refactor(t_coder *coder)
{
	print_status(coder, "is refactoring");
	smart_sleep(coder->env->t_refactor, coder->env);
}

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	//se l'ID è dispari introduco uno sfasamento
	if(coder->id % 2 == 0)
		usleep(1000);
	while (is_simulation_running(coder->env))
	{
		//DEBUG
		coder_debug(coder);
		if (!is_simulation_running(coder->env))
			break;
		//COMPILE
		coder_compile(coder);
		if (!is_simulation_running(coder->env))
			break;
		//Check sul numero di compilazioni richieste
		if (coder->env->req_compiles > 0 && coder->compile_count >= coder->env->req_compiles)
			break;
		//REFACTOR
		coder_refactor(coder);
	}
	return NULL;
}