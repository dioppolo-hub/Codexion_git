/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:09:14 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 10:42:53 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_n_coders(int n_coder)
{
	if (n_coder <= 0)
		return false;
	else if (n_coder > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_burnout(int t_burnout)
{
	if (t_burnout <= 0)
		return false;
	else if (t_burnout > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_compile(int t_compile)
{
	if (t_compile <= 0)
		return false;
	else if (t_compile > INT_MAX)
		return false;
	else
		return true;
}

bool parce_t_debug(int t_debug)
{
	if (t_debug <= 0)
		return false;
	else if (t_debug > INT_MAX)
		return false;
	else
		return true;
}

bool parcing1(int argc, char** argv)
{
	if (argc != 8)
		return false;
	if (
		parce_n_coders(argv[0]) &&\
		parce_t_burnout(argv[1]) &&\
		parce_t_compile(argv[2]) &&\
		parce_t_debug(argv[3]) &&\
		parce_t_refactor(argv[4]) &&\
		parce_n_comp(argv[5]) &&\
		parce_dongle_cool(argv[6]) &&\
		parce_scheduler(argv[7]))
		return true;
	else
		return false;
}