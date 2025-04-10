/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:39:49 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 12:02:58 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_cd_target(char **argv, t_env **env_list, char *oldpwd)
{
	char	*path;

	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
	{
		path = getenv("HOME");
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

int	builtin_cd(char **argv, t_env **env_list)
{
	char	cwd[1024];
	char	*path;
	char	*oldpwd;

	if (argv[1] && argv[2])
		return (ft_putendl_fd("cd: too many arguments", 2), 1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	path = get_cd_target(argv, env_list, oldpwd);
	if (!path || chdir(path) != 0)
		return (perror("cd"), free(oldpwd), 1);
	if (getcwd(cwd, sizeof(cwd)))
	{
		env_set(env_list, "OLDPWD", oldpwd);
		env_set(env_list, "PWD", cwd);
	}
	free(oldpwd);
	return (0);
}
