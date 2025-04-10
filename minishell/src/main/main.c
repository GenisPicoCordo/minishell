/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 11:20:25 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	t_shell	shell;

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: this program does not take arguments", 2);
		return (1);
	}
	env_list = env_init(envp);
	shell.env = envp;
	shell.last_status = 0;
	while (1)
	{
		shell.input = readline("minishell> ");
		if (!shell.input)
		{
			printf("exit\n");
			clean_exit(env_list, NULL, NULL, 0);
			break ;
		}
		if (*shell.input)
			add_history(shell.input);
		shell.tokens = tokenize_input(&shell);
		if (shell.tokens)
		{
			shell.cmd_table = parse_tokens(shell.tokens);
			if (shell.cmd_table)
			{
				if (!validate_cmd_table(&shell))
				{
					free_cmd_table(shell.cmd_table);
					free_tokens(shell.tokens);
					free(shell.input);
					continue ;
				}
				print_cmd_table(shell.cmd_table);
				free_cmd_table(shell.cmd_table);
			}
			shell.last_status = execute_tokens(shell.tokens, env_list);
			free_tokens(shell.tokens);
		}
		free(shell.input);
	}
	return (shell.last_status);
}
