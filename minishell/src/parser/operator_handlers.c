/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:14:02 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/08 14:55:12 by gpico-co         ###   ########.fr       */
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
	t_token		*token;
	const char	*input;
	int			next_index;

	input = shell->input;
	token = handle_logical_ops(input, &i);
	if (!token)
		token = handle_pipe(input, &i);
	if (!token)
		token = handle_redirect(input, &i);
	if (!token)
		return (INVALID_OPERATOR);
	next_index = skip_spaces(input, i + 1);
	if (!input[next_index] || ft_strchr("|<>", input[next_index]))
	{
		free(token->content);
		free(token);
		return (INVALID_OPERATOR);
	}
	append_token(head, tail, token);
	return (i + 1);
}
