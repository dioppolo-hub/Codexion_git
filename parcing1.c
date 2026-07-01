/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:09:14 by dioppolo          #+#    #+#             */
/*   Updated: 2026/07/01 16:01:51 by diego            ###   ########.fr       */
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

bool parcing_1(int argc, char** argv)
{
	if (argc != 9)
		return false;
	if (
		parce_n_coders(argv[1]) &&\
		parce_t_burnout(argv[2]) &&\
		parce_t_compile(argv[3]) &&\
		parce_t_debug(argv[4]) &&\
		parce_t_refactor(argv[5]) &&\
		parce_n_comp(argv[6]) &&\
		parce_dongle_cool(argv[7]) &&\
		parce_scheduler(argv[8]))
		return true;
	else
		return false;
}