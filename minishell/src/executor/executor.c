/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 14:20:01 by gpico-co         ###   ########.fr       */
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

void	execute_tokens(t_token *tokens, char **env, t_env **env_list)
{
	char	**argv;
	pid_t	pid;
	int		status;

	if (!tokens)
		return ;
	argv = build_argv(tokens);
	if (!argv)
		return ;
	if (is_builtin(argv[0]))
	{
		execute_builtin(argv, env_list);
		free(argv);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			execve(argv[0], argv, env);
			perror("execve");
			exit(1);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			free(argv);
		}
		else
			perror("fork");
	}
}
