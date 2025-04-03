/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:07:50 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/03 11:07:50 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd, "env", 4))
		return (1);
	if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

int	execute_builtin(char **argv, char **env)
{
	if (!argv || !argv[0])
		return (1);
	if (!ft_strncmp(argv[0], "echo", 5))
		return (builtin_echo(argv));
	if (!ft_strncmp(argv[0], "pwd", 4))
		return (builtin_pwd(argv));
	if (!ft_strncmp(argv[0], "env", 4))
		return (builtin_env(argv, env));
	if (!ft_strncmp(argv[0], "exit", 5))
		return (builtin_exit(argv));
	return (1);
}
