/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:05:46 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/03 15:50:36 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	str_is_overflowing_long(const char *str)
{
	unsigned long	res;
	unsigned long	limit;
	int				i;

	res = 0;
	i = 0;
	limit = (unsigned long)LONG_MAX;
	if (str[i] == '-')
	{
		limit += 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (res > (limit - (str[i] - '0')) / 10)
			return (1);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}
