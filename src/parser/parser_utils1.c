/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:06:52 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 18:15:20 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipes(t_token *tokens)
{
	int	count;

	count = 1;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	append_redir(t_token **redirs, t_token *redir)
{
	t_token	*new;
	t_token	*tmp;

	new = create_token(redir->content,
			ft_strlen(redir->content), redir->type);
	if (!new)
		return ;
	if (!*redirs)
		*redirs = new;
	else
	{
		tmp = *redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	parser_handle_word(t_cmd *cmd, char *content, int *arg_index)
{
	cmd->args[*arg_index] = ft_strdup(content);
	if (!cmd->cmd)
		cmd->cmd = ft_strdup(content);
	(*arg_index)++;
}

void	handle_redir(t_cmd *cmd, t_token **tmp)
{
	append_redir(&cmd->redirs, *tmp);
	if ((*tmp)->next && (*tmp)->next->type == T_WORD)
	{
		*tmp = (*tmp)->next;
		append_redir(&cmd->redirs, *tmp);
	}
}
