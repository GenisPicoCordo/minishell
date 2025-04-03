/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:44:04 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/03 15:49:45 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_exit(char **argv)
{
	long	code;

	if (!argv[1])
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	if (!is_numeric(argv[1]) || str_is_overflowing_long(argv[1]))
	{
		ft_putendl_fd("exit", 1);
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	if (argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	ft_putendl_fd("exit", 1);
	code = ft_atoi(argv[1]);
	exit((unsigned char)code);
}
