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

int	execute_tokens(t_token *tokens, t_env **env_list)
{
	char	**argv;
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	int		status;
	int		exit_code;

	if (!tokens)
		return (0);
	argv = build_argv(tokens);
	if (!argv || !argv[0])
		return (1);
	if (is_builtin(argv[0]))
	{
		exit_code = execute_builtin(argv, env_list);
		free(argv);
		return (exit_code);
	}
	else
	{
		cmd_path = find_command_path(argv[0]);
		env_array = env_to_array(*env_list);
		if (!cmd_path || !env_array)
		{
			ft_putendl_fd("Command not found", 2);
			free(argv);
			free(cmd_path);
			free_split(env_array);
			return (127);
		}
		pid = fork();
		if (pid == 0)
		{
			execve(cmd_path, argv, env_array);
			perror("execve");
			exit(1);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			free(argv);
			free(cmd_path);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			else
				return (1);
		}
		else
		{
			perror("fork");
			free(argv);
			free(cmd_path);
			return (1);
		}
	}
}
