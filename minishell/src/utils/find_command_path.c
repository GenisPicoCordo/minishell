/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:36:32 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 13:56:21 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Busca el path absoluto del comando usando el PATH del env_list.
 * Si el comando ya tiene un '/' (absoluto o relativo), se devuelve tal cual.
 * Si no encuentra el ejecutable, devuelve NULL.
 */

char	*join_path_cmd(char *path, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin_free(tmp, cmd);
	return (full);
}

char	*search_in_paths(char **paths, char *cmd)
{
	char	*full;
	int		i;

	i = 0;
	while (paths[i])
	{
		full = join_path_cmd(paths[i], cmd);
		if (full && access(full, X_OK) == 0)
		{
			free_split(paths);
			return (full);
		}
		free(full);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_var;
	char	**paths;

	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = env_get(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
