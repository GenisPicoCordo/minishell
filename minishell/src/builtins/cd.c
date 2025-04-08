/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:39:49 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/08 16:54:01 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_cd_args(char **argv)
{
	if (argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	return (0);
}

char	*get_cd_path(char **argv)
{
	char	*path;

	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		return (getenv("HOME"));
	if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			return (NULL);
		}
		ft_putendl_fd(path, 1);
		return (path);
	}
	return (argv[1]);
}

int	builtin_cd(char **argv)
{
	char	*path;
	char	cwd[1024];
	char	*oldpwd;

	if (validate_cd_args(argv))
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	path = get_cd_path(argv);
	if (!path || chdir(path) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	getcwd(cwd, sizeof(cwd));
	setenv("OLDPWD", oldpwd, 1);
	setenv("PWD", cwd, 1);
	free(oldpwd);
	return (0);
}
