/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:17:49 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/10 12:07:51 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cmd_exists(char *cmd)
{
	char	*path;

	if (!cmd)
		return (127);
	if (is_builtin(cmd))
		return (0);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (127);
		if (access(cmd, X_OK) != 0)
			return (126);
		return (0);
	}
	path = find_command_path(cmd);
	if (!path)
		return (127);
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (126);
	}
	free(path);
	return (0);
}

int	validate_cmd_table(t_shell *shell)
{
	int			i;
	t_cmd_table	*table;
	t_cmd		cmd;
	int			code;

	i = 0;
	table = shell->cmd_table;
	while (i < table->count)
	{
		cmd = table->cmds[i];
		if (!cmd.cmd)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|`", 2);
			shell->last_status = 2;
			return (0);
		}
		code = cmd_exists(cmd.cmd);
		if (code == 127)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd.cmd, 2);
			ft_putendl_fd(": command not found", 2);
			shell->last_status = 127;
			return (0);
		}
		if (code == 126)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd.cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			shell->last_status = 126;
			return (0);
		}
		i++;
	}
	return (1);
}
