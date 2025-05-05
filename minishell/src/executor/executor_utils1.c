/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:55:44 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/05 15:28:29 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_external(t_shell *shell, t_cmd *cmd, t_env **env_list)
{
	pid_t	pid;
	char	*cmd_path;
	char	**envp;

	cmd_path = find_command_path(cmd->cmd, *env_list);
	envp = env_to_array(*env_list);
	if (!cmd_path || !envp)
	{
		ft_putendl_fd("Command not found", 2);
		free(cmd_path);
		free_split(envp);
		return (127);
	}
	pid = fork();
	if (pid == 0)
		execve_child_process(cmd, cmd_path, envp);
	else if (pid > 0)
		return (wait_and_return_status(shell, pid, cmd_path, envp));
	perror("fork");
	free(cmd_path);
	free_split(envp);
	return (1);
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
	{
		if (apply_redirections(cmd->redirs))
			return (1);
		return (execute_builtin(cmd->args, env_list));
	}
	if (is_builtin(cmd->cmd))
		return (exec_builtin_in_child(cmd, env_list));
	return (exec_external(shell, cmd, env_list));
}

void	setup_redirs(t_cmd_table *t, t_pipeinfo *info)
{
	if (info->i != 0)
		dup2(info->in_fd, STDIN_FILENO);
	if (info->i < t->count - 1)
	{
		dup2(info->pipefd[1], STDOUT_FILENO);
		close(info->pipefd[0]);
		close(info->pipefd[1]);
	}
}

void	exec_builtin_or_external(t_cmd_table *t, t_env **env,
		t_pipeinfo *info)
{
	t_cmd	*cmd;
	char	*cmd_path;
	char	**envp;

	cmd = &t->cmds[info->i];
	if (apply_redirections(cmd->redirs))
		exit(1);
	if (is_builtin(cmd->cmd))
		exit(execute_builtin(cmd->args, env));
	cmd_path = find_command_path(cmd->cmd, *env);
	envp = env_to_array(*env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->cmd, 2);
		exit(127);
	}
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	exit(1);
}

void	child_setup(t_cmd_table *t, t_env **env, t_pipeinfo *info)
{
	setup_redirs(t, info);
	exec_builtin_or_external(t, env, info);
}
