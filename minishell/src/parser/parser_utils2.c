/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:07:58 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 18:27:00 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	fill_command_args(t_cmd *cmd, t_token **tmp, t_token *end)
{
	int	arg_count;
	int	arg_index;

	arg_count = count_args(*tmp);
	arg_index = 0;
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return ;
	while (*tmp != end && *tmp)
	{
		if ((*tmp)->type == T_WORD)
			parser_handle_word(cmd, (*tmp)->content, &arg_index);
		else if ((*tmp)->type >= T_REDIRECT_IN && (*tmp)->type <= T_HEREDOC)
			handle_redir(cmd, tmp);
		*tmp = (*tmp)->next;
	}
	cmd->args[arg_index] = NULL;
	cmd->heredoc_names = get_heredoc_names(cmd->redirs);
}

int	init_cmd_table(t_cmd_table **table, int count)
{
	*table = malloc(sizeof(t_cmd_table));
	if (!*table)
		return (0);
	(*table)->count = count;
	(*table)->cmds = malloc(sizeof(t_cmd) * count);
	if (!(*table)->cmds)
		return (0);
	return (1);
}

void	setup_command(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
}

void	move_to_next_pipe(t_token **current)
{
	while (*current && (*current)->type != T_PIPE)
		*current = (*current)->next;
}

void	fill_cmd_table(t_cmd_table *table, t_token *tokens)
{
	t_token	*start;
	t_token	*current;
	int		i;

	start = tokens;
	current = tokens;
	i = 0;
	while (i < table->count)
	{
		setup_command(&table->cmds[i]);
		move_to_next_pipe(&current);
		table->cmds[i].index = i;
		fill_command_args(&table->cmds[i], &start, current);
		if (current)
		{
			start = current->next;
			current = current->next;
		}
		i++;
	}
}
