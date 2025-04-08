/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:34:44 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/08 16:45:41 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	print_export_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x", 1);
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}
//aqui estamos usando putenv

int	process_export_arg(char *arg)
{
	if (!ft_strchr(arg, '=') || !is_valid_identifier(arg))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	putenv(arg);
	return (0);
}

int	builtin_export(char **argv, char **env)
{
	int	i;

	if (!argv[1])
		return (print_export_env(env));
	i = 1;
	while (argv[i])
	{
		process_export_arg(argv[i]);
		i++;
	}
	return (0);
}
