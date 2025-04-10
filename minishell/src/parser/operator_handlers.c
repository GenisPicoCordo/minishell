/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:14:02 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/10 11:55:52 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*handle_logical_ops(const char *input, int *i)
{
	if (input[*i] == '|' && input[*i + 1] == '|')
	{
		(*i)++;
		return (create_token("||", 2, T_OR));
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		(*i)++;
		return (create_token("&&", 2, T_AND));
	}
	return (NULL);
}

t_token	*handle_pipe(const char *input, int *i)
{
	if (input[*i] == '|')
		return (create_token("|", 1, T_PIPE));
	return (NULL);
}

t_token	*handle_redirect(const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		(*i)++;
		return (create_token("<<", 2, T_HEREDOC));
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		(*i)++;
		return (create_token(">>", 2, T_APPEND));
	}
	else if (input[*i] == '<')
		return (create_token("<", 1, T_REDIRECT_IN));
	else if (input[*i] == '>')
		return (create_token(">", 1, T_REDIRECT_OUT));
	return (NULL);
}

int	handle_operator(t_shell *shell, int i, t_token **head, t_token **tail)
{
	t_token	*token;

	token = handle_logical_ops(shell->input, &i);
	if (!token)
		token = handle_pipe(shell->input, &i);
	if (!token)
		token = handle_redirect(shell->input, &i);
	if (!token)
		return (INVALID_OPERATOR);
	append_token(head, tail, token);
	return (i + 1);
}
