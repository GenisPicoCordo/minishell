/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:14:02 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 14:20:32 by ncampo-f         ###   ########.fr       */
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

int	handle_operator(const char *input, int i, t_lexer_ctx *ctx)
{
	t_token	*token;

	token = handle_logical_ops(input, &i);
	if (!token)
		token = handle_pipe(input, &i);
	if (!token)
		token = handle_redirect(input, &i);
	if (!token)
		return (INVALID_OPERATOR);
	append_token(ctx->head, ctx->tail, token);
	return (i + 1);
}
