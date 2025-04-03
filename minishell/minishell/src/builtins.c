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

#include "../includes/minishell.h"

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
	return (1);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (argv[i] && !ft_strncmp(argv[i], "-n", 3))
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

int	builtin_pwd(char **argv)
{
	char	cwd[1024];

	if (argv[1])
	{
		ft_putendl_fd("pwd: to many arguments", 2);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	builtin_env(char **argv, char **env)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		ft_putendl_fd("env: to many arguments", 2);
		return (127);
	}
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return (0);
}
