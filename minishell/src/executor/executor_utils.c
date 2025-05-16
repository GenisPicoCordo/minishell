/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:01:36 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 16:04:05 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_parent_builtin(t_cmd *cmd, t_env **env_list)
{
	int	in_backup;
	int	out_backup;
	int	status;

	in_backup = dup(STDIN_FILENO);
	out_backup = dup(STDOUT_FILENO);
	if (apply_redirections(cmd->redirs))
	{
		close(in_backup);
		close(out_backup);
		return (1);
	}
	status = execute_builtin(cmd->args, env_list);
	dup2(in_backup, STDIN_FILENO);
	dup2(out_backup, STDOUT_FILENO);
	close(in_backup);
	close(out_backup);
	return (status);
}

int	execute_tokens(t_shell *shell, t_cmd_table *table, t_env **env_list)
{
	t_cmd	*cmd;

	if (!table || table->count != 1)
		return (1);
	cmd = &table->cmds[0];
	if (!cmd->cmd)
		return (1);
	if (is_parent_builtin(cmd->cmd))
		return (exec_parent_builtin(cmd, env_list));
	if (is_builtin(cmd->cmd))
		return (exec_builtin_in_child(cmd, env_list));
	return (exec_external(shell, cmd, env_list));
}
