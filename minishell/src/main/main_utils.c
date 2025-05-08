/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:11:13 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 13:57:28 by gpico-co         ###   ########.fr       */
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

int	free_and_interrupt(t_shell *shell)
{
	shell->last_status = 130;
	if (shell->cmd_table)
	{
		free_cmd_table(shell->cmd_table);
		shell->cmd_table = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	return (1);
}

int	handle_interrupt_and_errors(t_shell *shell)
{
	if (signal_flag(GET, 0) == SHELL_INTERRUPTED)
	{
		shell->last_status = 130;
		signal_flag(SET, SHELL_NORMAL);
		free(shell->input);
		shell->input = NULL;
		return (1);
	}
	exit_if_eof(shell);
	return (0);
}

void	main_loop(t_shell *shell)
{
	int	ret;

	while (1)
	{
		signal_flag(SET, SHELL_NORMAL);
		shell->input = readline("minishell> ");
		if (handle_interrupt_and_errors(shell))
			continue ;
		if (*shell->input)
			add_history(shell->input);
		shell->tokens = tokenize_input(shell);
		ret = execute_command_loop(shell);
		free(shell->input);
		shell->input = NULL;
		if (ret)
			continue ;
	}
}
