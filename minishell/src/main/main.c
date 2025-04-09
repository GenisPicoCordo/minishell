/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 15:23:37 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	maintance(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	int		last_status;

	(void)argc;
	(void)argv;
	last_status = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = tokenize_input(input, envp, last_status);
		if (tokens)
		{
			last_status = execute_tokens(tokens, envp);
			free_tokens(tokens);
		}
		free(input);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_cmd_table	*cmd_table;
	int			last_status = 0;

	(void)argc;
	(void)argv;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);

		tokens = tokenize_input(input, envp, last_status);
		if (tokens)
		{
			cmd_table = parse_tokens(tokens);
			if (cmd_table)
			{
				if (!validate_cmd_table(cmd_table, envp))
				{
					free_cmd_table(cmd_table);
					free_tokens(tokens);
					free(input);
					continue ;
				}
				print_cmd_table(cmd_table); // ← solo mientras estés probando
				free_cmd_table(cmd_table);
			}
			free_tokens(tokens);
		}
		free(input);
	}
	return (last_status);
}
