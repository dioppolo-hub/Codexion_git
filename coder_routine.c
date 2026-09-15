/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 17:16:42 by diego             #+#    #+#             */
/*   Updated: 2026/09/15 09:49:43 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->env->write_mutex);
	pthread_mutex_lock(&coder->env->sim_mutex);
	if (coder->env->simulation_running)
	{
		timestamp = get_time_ms() - coder->env->start_time;
		printf("%lld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->env->sim_mutex);
	pthread_mutex_unlock(&coder->env->write_mutex);
}

//controllo sullo stato della simulzione
bool	is_simulation_running(t_env *env)
{
	bool	running;

	pthread_mutex_lock(&env->sim_mutex);
	running = env->simulation_running;
	pthread_mutex_unlock(&env->sim_mutex);
	return (running);
}

//sleep per rispondere alla fine della simulazione
void	smart_sleep(long long time_in_ms, t_env *env)
{
	long long	start;

	start = get_time_ms();
	while (is_simulation_running(env))
	{
		if (get_time_ms() - start >= time_in_ms)
			break ;
		usleep(500);
	}
}

void	coder_debug(t_coder *coder)
{
	print_status(coder, "is debugging");
	smart_sleep(coder->env->t_debug, coder->env);
}

//prendi dongle
//aggiorna l'orario di last compilazione
//compila
//cont++
//lascia i dongle
void	coder_compile(t_coder *coder)
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

void	coder_refactor(t_coder *coder)
{
	print_status(coder, "is refactoring");
	smart_sleep(coder->env->t_refactor, coder->env);
}

//se l'ID è dispari introduco uno sfasamento
//DEBUG
//COMPILE
//Check sul numero di compilazioni richieste
//REFACTOR
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (is_simulation_running(coder->env))
	{
		coder_debug(coder);
		if (!is_simulation_running(coder->env))
			break ;
		coder_compile(coder);
		if (!is_simulation_running(coder->env))
			break ;
		if (
			coder->env->req_compiles > 0
			&& coder->compile_count >= coder->env->req_compiles
		)
			break ;
		coder_refactor(coder);
	}
	return (NULL);
}
