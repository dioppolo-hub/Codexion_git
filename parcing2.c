/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:43:25 by dioppolo          #+#    #+#             */
/*   Updated: 2026/07/01 16:02:01 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_t_refactor(char *t_refactor)
{
	long int x;

	if (!is_valid_int(t_refactor, true))
		return false;
	x = atoi(t_refactor);
	if (x < 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_n_comp(char *n_comp)
{
	long int x;

	if (!is_valid_int(n_comp, false))
		return false;
	x = atoi(n_comp);
	if (x <= 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_dongle_cool(char *d_cooldown)
{
	long int x;

	if (!is_valid_int(d_cooldown, true))
		return false;
	x = atoi(d_cooldown);
	if (x < 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_scheduler(char *scheduler)
{
	if (strcmp(scheduler, "fifo") != 0 &&\
		strcmp(scheduler, "edf") != 0)
		return false;
	else
		return true;
}