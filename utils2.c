/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:30:39 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/21 11:18:51 by dioppolo         ###   ########.fr       */
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
