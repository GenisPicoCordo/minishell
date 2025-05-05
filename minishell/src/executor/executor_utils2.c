/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:57:10 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/05 15:49:45 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_parent_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "exit", 5));
}

static void	handle_child_builtin(t_cmd *cmd, t_env **env_list)
{
	int	status;

	signal_flag(SET, SHELL_CHILD);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd->redirs))
		exit(1);
	status = execute_builtin(cmd->args, env_list);
	if (cmd->redirs)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	exit(status);
}

int	exec_builtin_in_child(t_cmd *cmd, t_env **env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		handle_child_builtin(cmd, env_list);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	wait_and_return_status(t_shell *shell, int pid, char *cmd_path, char **envp)
{
	int	status;
	int	exit_code;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free_split(envp);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		exit_code = 128 + WTERMSIG(status);
	}
	else
		exit_code = 1;
	if (shell->info)
		close(shell->info->pipefd[0]);
	if (shell->info)
		close(shell->info->pipefd[1]);
	return (exit_code);
}

void	execve_child_process(t_cmd *cmd, char *cmd_path, char **envp)
{
	signal_flag(SET, SHELL_CHILD);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd->redirs))
		exit(1);
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	if (cmd->redirs)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	exit(1);
}
