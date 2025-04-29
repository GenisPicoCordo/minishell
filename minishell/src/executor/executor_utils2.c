/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:57:10 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 14:58:04 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**build_argv(t_token *tokens)
{
	int		i;
	int		count;
	char	**argv;

	i = 0;
	count = count_tokens(tokens);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == T_COMMANDS || tokens->type == T_ARGUMENT
			|| tokens->type == T_WORD)
			argv[i++] = tokens->content;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	is_parent_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "exit", 5));
}

int	exec_builtin_in_child(t_cmd *cmd, t_env **env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(cmd->redirs))
			exit(1);
		exit(execute_builtin(cmd->args, env_list));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	wait_and_return_status(int pid, char *cmd_path, char **envp)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free_split(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

void	execve_child_process(t_cmd *cmd, char *cmd_path, char **envp)
{
	if (apply_redirections(cmd->redirs))
		exit(1);
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	exit(1);
}
