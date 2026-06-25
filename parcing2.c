/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dioppolo <dioppolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 10:43:25 by dioppolo          #+#    #+#             */
/*   Updated: 2026/06/25 10:48:28 by dioppolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool parce_t_refactor(int t_refactor)
{
	if (t_refactor <= 0)
		return false;
	else if (t_refactor > INT_MAX)
		return false;
	else
		return true;
}

bool parce_n_comp(int n_comp)
{
	if (n_comp <= 0)
		return false;
	else if (n_comp > INT_MAX)
		return false;
	else
		return true;
}

bool parce_dongle_cool(int d_cooldown)
{
	if (d_cooldown <= 0)
		return false;
	else if (d_cooldown > INT_MAX)
		return false;
	else
		return true;
}

bool parce_scheduler(char *scheduler)
{
	if (scheduler != "fifo" && scheduler != "edf")
		return false;
	else
		return true;
}