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
	if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd, "export", 7))
		return (1);
	if (!ft_strncmp(cmd, "unset", 7))
		return (1);
	return (0);
}

int	execute_builtin(char **argv, t_env **env_list)
{
	if (!argv || !argv[0])
		return (1);
	if (!ft_strncmp(argv[0], "echo", 5))
		return (builtin_echo(argv));
	if (!ft_strncmp(argv[0], "pwd", 4))
		return (builtin_pwd(argv));
	if (!ft_strncmp(argv[0], "env", 4))
		return (builtin_env(argv, *env_list));
	if (!ft_strncmp(argv[0], "exit", 5))
		return (builtin_exit(argv));
	if (!ft_strncmp(argv[0], "cd", 3))
		return (builtin_cd(argv, env_list));
	if (!ft_strncmp(argv[0], "export", 7))
		return (builtin_export(argv, env_list));
	if (!ft_strncmp(argv[0], "unset", 6))
		return (builtin_unset(argv, env_list));
	return (1);
}
