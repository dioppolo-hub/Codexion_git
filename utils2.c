/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:30:39 by dioppolo          #+#    #+#             */
/*   Updated: 2026/10/14 10:34:51 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_both_dongle(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long long	av_at;

	right = coder->right_dongle;
	left = coder->left_dongle;
	pthread_mutex_lock(&coder->env->pair_mutex);
	av_at = get_time_ms() + coder->env->cooldown;
	left->is_in_use = false;
	right->is_in_use = false;
	left->available_at = av_at;
	right->available_at = av_at;
	pthread_cond_broadcast(&coder->env->pair_cond);
	pthread_mutex_unlock(&coder->env->pair_mutex);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	grant_pair(t_coder *coder)
{
	t_env	*env;

	env = coder->env;
	heap_pop(&env->pair_heap, env->scheduler_type);
	coder->left_dongle->is_in_use = true;
	coder->right_dongle->is_in_use = true;
	pthread_cond_broadcast(&env->pair_cond);
}

bool	is_pair_turn(t_coder *coder, t_request top)
{
	return (top.coder_id == coder->id && pair_is_free(coder));
}

void	check_pair_turn(t_coder *coder, long long rem, t_request top)
{
	t_env	*env;

	env = coder->env;
	if (is_pair_turn(coder, top) && rem > 0)
		wait_pair_cooldown(env, rem);
	else
		pthread_cond_wait(&env->pair_cond, &env->pair_mutex);
}
