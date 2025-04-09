/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:39:49 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 15:30:17 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_cd(char **argv, t_env **env_list)
{
	char	cwd[1024];
	char	*path;
	char	*oldpwd;

	if (argv[1] && argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		path = env_get(*env_list, "HOME");
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		path = env_get(*env_list, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			free(oldpwd);
			return (1);
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = argv[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		env_set(env_list, "OLDPWD", oldpwd);
		env_set(env_list, "PWD", cwd);
	}
	free(oldpwd);
	return (0);
}
