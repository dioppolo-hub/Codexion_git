/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:47:22 by dioppolo          #+#    #+#             */
/*   Updated: 2026/09/15 10:47:34 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_dongles(t_env *env)
{
	int	i;

	env->dongles = malloc(sizeof(t_dongle) * env->num_coders);
	if (!env->dongles)
		return (false);
	i = 0;
	while (i < env->num_coders)
	{
		env->dongles[i].id = i;
		env->dongles[i].is_in_use = false;
		env->dongles[i].last_released_time = 0;
		if (pthread_mutex_init(&env->dongles[i].mutex, NULL) != 0)
			return (false);
		if (pthread_cond_init(&env->dongles[i].cond, NULL) != 0)
			return (false);
		heap_init(&env->dongles[i].heap, env->num_coders);
		i++;
	}
	return (true);
}

t_coder	*init_coders(t_env *env)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * env->num_coders);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < env->num_coders)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_start = env->start_time;
		coders[i].env = env;
		coders[i].left_dongle = &env->dongles[i];
		coders[i].right_dongle = &env->dongles[(i + 1) % env->num_coders];
		i++;
	}
	return (coders);
}
