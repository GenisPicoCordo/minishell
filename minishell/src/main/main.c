/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 16:33:30 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: this program does not take arguments", 2);
		return (1);
	}
	setup_signal_handlers();
	shell.env_list = env_init(envp);
	shell.last_status = 0;

	while (1)
	{
		signal_flag(SET, SHELL_NORMAL);
		shell.input = readline("minishell> ");
		if (!shell.input)
		{
			write(1, "exit\n", 5);
			clean_exit(shell.env_list, NULL, NULL, 0);
		}
		if (*shell.input)
			add_history(shell.input);

		shell.tokens = tokenize_input(&shell);
		if (shell.tokens)
		{
			shell.cmd_table = parse_tokens(shell.tokens);
			if (shell.cmd_table && validate_cmd_table(&shell))
			{
				if (preprocess_heredocs(shell.cmd_table))
				{
					free_cmd_table(shell.cmd_table);
					free_tokens(shell.tokens);
					free(shell.input);
					continue;
				}
				if (shell.cmd_table->count == 1)
					shell.last_status = execute_tokens(shell.cmd_table, &shell.env_list);
				else
					shell.last_status = execute_pipeline(shell.cmd_table, &shell.env_list);
			}
			free_cmd_table(shell.cmd_table);
			free_tokens(shell.tokens);
		}
		free(shell.input);
	}
	return (shell.last_status);
}
