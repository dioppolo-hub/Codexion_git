/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:17:47 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/16 12:40:03 by diego            ###   ########.fr       */
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
	if (coder->left_dongle->id < coder->right_dongle->id)	
	{
		lock_left_dongles(coder);
		lock_right_dongles(coder);
	}
	else
	{
		lock_right_dongles(coder);
		lock_left_dongles(coder);
	}
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
	while (is_simulation_running(coder->env))
	{
		coder_compile(coder);
		if (!is_simulation_running(coder->env))
			break ;
		coder_debug(coder);
		if (!is_simulation_running(coder->env))
			break ;
		coder_refactor(coder);
		if (
			coder->env->req_compiles > 0
			&& coder->compile_count >= coder->env->req_compiles
		)
			break ;
	}
	return (NULL);
}
