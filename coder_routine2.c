/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:17:47 by dioppolo          #+#    #+#             */
/*   Updated: 2026/10/09 10:12:18 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//prendi dongle
//aggiorna l'orario di last compilazione
//compila
//cont++
//lascia i dongle
void	coder_compile(t_coder *coder)
{
	wait_initial_phase(coder);
	if (!is_simulation_running(coder->env))
		return ;
	pthread_mutex_lock(&coder->env->sim_mutex);
	if (!coder->deadline_active)
	{
		coder->deadline = get_time_ms() + coder->env->t_burnout;
		coder->deadline_active = true;
	}
	pthread_mutex_unlock(&coder->env->sim_mutex);
	lock_both_dongles(coder);
	print_status(coder, "is compiling");
	smart_sleep(coder->env->t_compile, coder->env);
	pthread_mutex_lock(&coder->env->sim_mutex);
	if (coder->env->simulation_running)
		coder->deadline = get_time_ms() + coder->env->t_burnout;
	pthread_mutex_unlock(&coder->env->sim_mutex);
	release_both_dongle(coder);
	complete_initial_phase(coder);
}

void	coder_refactor(t_coder *coder)
{
	smart_sleep(coder->env->t_refactor, coder->env);
	print_status(coder, "is refactoring");
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
	while (is_simulation_running(coder->env))
	{
		coder_compile(coder);
		if (!is_simulation_running(coder->env))
			break ;
		coder_debug(coder);
		if (!is_simulation_running(coder->env))
			break ;
		coder_refactor(coder);
		pthread_mutex_lock(&coder->env->sim_mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->env->sim_mutex);
		if (
			coder->env->req_compiles > 0
			&& coder->compile_count >= coder->env->req_compiles
		)
			break ;
	}
	return (NULL);
}
