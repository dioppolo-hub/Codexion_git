/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:09:14 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 11:32:53 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_n_coders(char *n_coder)
{
	long int x;

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

	x = atoi(t_debug);
	if (x <= 0)
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