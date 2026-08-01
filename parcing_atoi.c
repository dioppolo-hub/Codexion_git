/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diego <diego@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:51:54 by diego             #+#    #+#             */
/*   Updated: 2026/08/01 19:05:56 by diego            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long ft_atoll(const char *str)
{
	long long	result;
	int			i;
	int			sign;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

bool is_valid_int(char *str, bool allow_zero)
{
	int i = 0;
	int val;

	if (!str || str[0] == '\0')
		return false;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
		i++;
	}
	val = ft_atoll(str);
	if (val > INT_MAX)
		return false;
	if (allow_zero && val < 0)
		return false;
	if (!allow_zero && val <= 0)
		return false;
	return true;
}