/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:43:25 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 11:33:03 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_t_refactor(char *t_refactor)
{
	long int x;

	x = atoi(t_refactor);
	if (x <= 0)
		return false;
	else if (x > INT_MAX)
		return false;
	else
		return true;
}

bool parce_n_comp(char *n_comp)
{
	long int x;

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

	x = atoi(d_cooldown);
	if (x <= 0)
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