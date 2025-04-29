/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:17:49 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/29 19:21:15 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int	cmd_exists(char *cmd, t_env *env_list)
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
	path = find_command_path(cmd, env_list);
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

static int	handle_cmd_error(t_shell *shell, char *cmd, int code)
{
	if (code == 127)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
		shell->last_status = 127;
		return (0);
	}
	if (code == 126)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Permission denied", 2);
		shell->last_status = 126;
		return (0);
	}
	return (1);
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
			ft_putendl_fd("minishell: syntax error near \
				unexpected token `|`", 2);
			shell->last_status = 2;
			return (0);
		}
		code = cmd_exists(cmd.cmd, shell->env_list);
		if (!handle_cmd_error(shell, cmd.cmd, code))
			return (0);
		i++;
	}
	return (1);
}
