/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:11:13 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/05 14:28:29 by gpico-co         ###   ########.fr       */
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
	free_cmd_table(shell->cmd_table);
	free_tokens(shell->tokens);
	free(shell->input);
	shell->cmd_table = NULL;
	shell->tokens = NULL;
	shell->input = NULL;
	return (1);
}

int	execute_command_loop(t_shell *shell)
{
	int	error;

	if (!shell->tokens)
		return (0);
	g_shell_state = SHELL_CHILD;
	shell->cmd_table = parse_tokens(shell->tokens);
	if (!shell->cmd_table || !validate_cmd_table(shell))
		return (0);
	error = preprocess_heredocs(shell->cmd_table);
	if (error || signal_flag(GET, 0) == SHELL_HEREDOC_INTERRUPTED)
		return (free_and_interrupt(shell));
	if (shell->cmd_table->count == 1)
		shell->last_status = execute_tokens(shell, shell->cmd_table,
				&shell->env_list);
	else
		shell->last_status = execute_pipeline(shell, shell->cmd_table, \
			&shell->env_list);
	free_cmd_table(shell->cmd_table);
	free_tokens(shell->tokens);
	return (0);
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
	if (has_unsupported_chars(shell->input))
	{
		ft_putendl_fd("minishell: unsupported character: ';' '\\' '\\n'", 2);
		free(shell->input);
		shell->input = NULL;
		return (1);
	}
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
		if (ret)
			continue ;
	}
}
