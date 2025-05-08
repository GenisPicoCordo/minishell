/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:40:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 13:53:13 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	should_skip_execution(t_shell *shell)
{
	if (!shell->cmd_table || !validate_cmd_table(shell))
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return (1);
	}
	return (0);
}

void	cleanup_after_execution(t_shell *shell)
{
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
}

int	execute_command_loop(t_shell *shell)
{
	int	error;

	if (!shell->tokens)
		return (0);
	g_shell_state = SHELL_CHILD;
	shell->cmd_table = parse_tokens(shell->tokens);
	if (!shell->cmd_table)
		return (0);
	if (should_skip_execution(shell))
	{
		free_cmd_table(shell->cmd_table);
		shell->cmd_table = NULL;
		return (0);
	}
	error = preprocess_heredocs(shell->cmd_table);
	if (error || signal_flag(GET, 0) == SHELL_HEREDOC_INTERRUPTED)
		return (free_and_interrupt(shell));
	if (shell->cmd_table->count == 1)
		shell->last_status = execute_tokens(shell,
				shell->cmd_table, &shell->env_list);
	else
		shell->last_status = execute_pipeline(shell->cmd_table,
				&shell->env_list);
	cleanup_after_execution(shell);
	return (0);
}
