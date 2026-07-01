/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:51:54 by diego             #+#    #+#             */
/*   Updated: 2026/07/01 16:03:03 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool is_valid_int(char *str, bool allow_zero)
{
	int i = 0;
	int len = 0;
	int val;

	if (!str || str[0] == '\0')
		return false;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
		i++;
		len++;
	}
	if (len > 10)
		return false;
	val = atoi(str);
	if (allow_zero && val < 0)
		return false;
	if (allow_zero && val <= 0)
		return false;
	return true;
}