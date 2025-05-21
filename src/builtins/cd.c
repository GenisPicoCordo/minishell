/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:39:49 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/19 18:05:11 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_cd_target(char **argv, t_env **env_list, char *oldpwd)
{
	char	*path;

	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
	{
		path = env_get(*env_list, "HOME");
		if (!path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (NULL);
		}
		return (path);
	}
	if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		path = env_get(*env_list, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			free(oldpwd);
			return (NULL);
		}
		ft_putendl_fd(path, 1);
		return (path);
	}
	return (argv[1]);
}

static int	cd_too_many_args(char **argv)
{
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	return (0);
}

static int	cd_update_env(t_env **env_list, char *oldpwd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		env_set(env_list, "OLDPWD", oldpwd);
		env_set(env_list, "PWD", cwd);
		return (0);
	}
	return (1);
}

int	builtin_cd(char **argv, t_env **env_list)
{
	char	*path;
	char	*oldpwd;

	if (cd_too_many_args(argv))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	path = get_cd_target(argv, env_list, oldpwd);
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	cd_update_env(env_list, oldpwd);
	free(oldpwd);
	return (0);
}
