/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:36:32 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:06:40 by ncampo-f         ###   ########.fr       */
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
