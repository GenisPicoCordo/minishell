/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:11:13 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 15:11:32 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_if_eof(t_shell *shell)
{
	if (!shell->input)
	{
		write(1, "exit\n", 5);
		clean_exit(shell->env_list, NULL, NULL, 0);
	}
}

void	execute_command_loop(t_shell *shell)
{
	if (!shell->tokens)
		return ;
	shell->cmd_table = parse_tokens(shell->tokens);
	if (shell->cmd_table && validate_cmd_table(shell))
	{
		if (preprocess_heredocs(shell->cmd_table))
		{
			free_cmd_table(shell->cmd_table);
			free_tokens(shell->tokens);
			free(shell->input);
			return ;
		}
		if (shell->cmd_table->count == 1)
			shell->last_status = execute_tokens(shell->cmd_table, \
				&shell->env_list);
		else
			shell->last_status = execute_pipeline(shell->cmd_table, \
				&shell->env_list);
	}
	free_cmd_table(shell->cmd_table);
	free_tokens(shell->tokens);
}

void	main_loop(t_shell *shell)
{
	while (1)
	{
		signal_flag(SET, SHELL_NORMAL);
		shell->input = readline("minishell> ");
		exit_if_eof(shell);
		if (*shell->input)
			add_history(shell->input);
		shell->tokens = tokenize_input(shell);
		execute_command_loop(shell);
		free(shell->input);
	}
}
