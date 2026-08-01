/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:09:14 by dioppolo          #+#    #+#             */
/*   Updated: 2026/08/01 18:58:25 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_n_coders(char *n_coder)
{
	long int x;

	if (!is_valid_int(n_coder, false))
		return false;
	x = atoi(n_coder);
	if (x <= 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_burnout(char *t_burnout)
{
	long int x;

	if (!is_valid_int(t_burnout, false))
		return false;
	x = atoi(t_burnout);
	if (x <= 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_compile(char *t_compile)
{
	long int x;

	if (!is_valid_int(t_compile, false))
		return false;
	x = atoi(t_compile);
	if (x <= 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_debug(char *t_debug)
{
	long int x;

	if(!is_valid_int(t_debug, true))
		return false;
	x = atoi(t_debug);
	if (x < 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parcing_1(int argc, char** argv, t_env *env)
{
	if (argc != 9)
		return false;
	if (
		!parce_n_coders(argv[1]) ||\
		!parce_t_burnout(argv[2]) ||\
		!parce_t_compile(argv[3]) ||\
		!parce_t_debug(argv[4]) ||\
		!parce_t_refactor(argv[5]) ||\
		!parce_n_comp(argv[6]) ||\
		!parce_dongle_cool(argv[7]) ||\
		!parce_scheduler(argv[8]))
		return false;
	env->num_coders = atoi(argv[1]);
	env->t_burnout = atoi(argv[2]);
	env->t_compile = atoi(argv[3]);
	env->t_debug = atoi(argv[4]);
	env->t_refactor = atoi(argv[5]);
	env->req_compiles = atoi(argv[6]);
	env->cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "edf") == 0)
		env->scheduler_type = 1;
	else if (strcmp(argv[8], "fifo") == 0)
		env->scheduler_type = 0;
	else
		return false;
	return true;
}