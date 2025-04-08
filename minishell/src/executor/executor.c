/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/08 15:56:20 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
🛠️ Paso 1: Preparar un subcomando de prueba
Vamos a asumir que el parser de tu compañero te da una lista de este tipo:

// tokens: ls -l -a
[t_token] -> content: "ls"   | type: T_COMMANDS  
[t_token] -> content: "-l"   | type: T_ARGUMENT  
[t_token] -> content: "-a"   | type: T_ARGUMENT  
Lo que necesitamos es:

Construir un char **argv para execve

Verificar si es un builtin (por ahora no)

Si no → hacer fork, y el hijo ejecuta con execve
*/

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

int	execute_tokens(t_token *tokens, char **env)
{
	char	**argv;
	char	*cmd_path;
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
		exit_code = execute_builtin(argv, env);
		free(argv);
		return (exit_code);
	}
	else
	{
		cmd_path = find_command_path(argv[0]);
		if (!cmd_path)
		{
			ft_putendl_fd("Command not found", 2);
			free(argv);
			return (127); // estándar bash: 127 = command not found
		}
		pid = fork();
		if (pid == 0)
		{
			execve(cmd_path, argv, env);
			perror("execve");
			exit(1); // solo el hijo termina así
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			free(argv);
			free(cmd_path);
			if (WIFEXITED(status))
				return WEXITSTATUS(status);
			else
				return 1;
		}
		else
		{
			perror("fork");
			free(argv);
			free(cmd_path);
			return 1;
		}
	}
}
