/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 12:33:25 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(t_token *list)
{
	int	count;

	count = 0;
	while (list)
	{
		if (list->type == T_COMMANDS || list->type == T_ARGUMENT)
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
		if (tokens->type == T_COMMANDS || tokens->type == T_ARGUMENT)
			argv[i++] = tokens->content;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	execute_tokens(t_token *tokens, char **env_unused, t_env **env_list)
{
	char	**argv;
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	int		status;

	(void)env_unused;
	if (!tokens)
		return ;
	argv = build_argv(tokens);
	if (!argv)
		return ;
	if (is_builtin(argv[0]))
	{
		execute_builtin(argv, env_list);
		free(argv);
		return ;
	}
	cmd_path = find_command_path(argv[0], *env_list);
	env_array = env_to_array(*env_list);
	if (!cmd_path || !env_array)
	{
		perror("minishell");
		free(argv);
		free(cmd_path);
		free_split(env_array);
		return ;
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
	}
	else
		perror("fork");
	free(argv);
	free(cmd_path);
	free_split(env_array);
}
