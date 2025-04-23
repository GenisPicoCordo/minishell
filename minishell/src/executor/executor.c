/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:11:02 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(t_token *list)
{
	int	count;

	count = 0;
	while (list)
	{
		if (list->type == T_COMMANDS || list->type == T_ARGUMENT || list->type == T_WORD)
			count++;
		list = list->next;
	}
	return (count);
}

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
		if (tokens->type == T_COMMANDS || tokens->type == T_ARGUMENT || tokens->type == T_WORD)
			argv[i++] = tokens->content;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

int execute_tokens(t_cmd_table *table, t_env **env_list)
{
	t_cmd     cmd;
	char     *cmd_path;
	char    **env_array;
	pid_t     pid;
	int       status;

	if (!table || table->count != 1)
		return (1); // Solo ejecutar si hay un solo comando

	cmd = table->cmds[0];
	if (!cmd.cmd)
		return (1);

	// Builtins que deben ejecutarse en el proceso padre
	if (!ft_strncmp(cmd.cmd, "cd", 3)
		|| !ft_strncmp(cmd.cmd, "export", 7)
		|| !ft_strncmp(cmd.cmd, "unset", 6)
		|| !ft_strncmp(cmd.cmd, "exit", 5))
	{
		if (apply_redirections(cmd.redirs))
			return (1);
		return execute_builtin(cmd.args, env_list);
	}

	// Otros builtins que pueden ejecutarse en un hijo
	if (is_builtin(cmd.cmd))
	{
		pid = fork();
		if (pid == 0)
		{
			if (apply_redirections(cmd.redirs))
				exit(1);
			int exit_code = execute_builtin(cmd.args, env_list);
			exit(exit_code);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
		}
		else
		{
			perror("fork");
			return 1;
		}
	}

	// Comando externo
	cmd_path = find_command_path(cmd.cmd);
	env_array = env_to_array(*env_list);

	if (!cmd_path || !env_array)
	{
		ft_putendl_fd("Command not found", 2);
		free(cmd_path);
		free_split(env_array);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(cmd.redirs))
			exit(1);
		execve(cmd_path, cmd.args, env_array);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		free_split(env_array);
		return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	}
	else
	{
		perror("fork");
		free(cmd_path);
		free_split(env_array);
		return 1;
	}
}


int	execute_pipeline(t_cmd_table *table, t_env **env_list)
{
	int		i;
	int		pipefd[2];
	int		in_fd = 0; // input inicial (stdin)
	pid_t	pid;

	i = 0;
	while (i < table->count)
	{
		if (i < table->count - 1)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			// Redirección del input
			if (i != 0)
				dup2(in_fd, STDIN_FILENO);
			// Redirección del output (solo si no es el último)
			if (i < table->count - 1)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			if (apply_redirections(table->cmds[i].redirs))
				exit(1);
			if (is_builtin(table->cmds[i].cmd))
			{
				int exit_code = execute_builtin(table->cmds[i].args, env_list);
				exit(exit_code);
			}
			else
			{
				char *cmd_path = find_command_path(table->cmds[i].cmd);
				char **envp = env_to_array(*env_list);
				if (!cmd_path)
				{
					ft_putstr_fd("minishell: command not found: ", 2);
					ft_putendl_fd(table->cmds[i].cmd, 2);
					exit(127);
				}
				execve(cmd_path, table->cmds[i].args, envp);
				perror("execve");
				exit(1);
			}
		}
		else if (pid < 0)
			perror("fork");
		if (i != 0)
			close(in_fd); // cerramos input anterior
		if (i < table->count - 1)
		{
			close(pipefd[1]);
			in_fd = pipefd[0]; // el próximo proceso leerá de aquí
		}
		i++;
	}
	// Esperar a todos los hijos
	while (wait(NULL) > 0);
	return (0);
}
