/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:20 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/14 17:06:38 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_tokens(t_shell *shell, t_token **head, t_token **tail)
{
	int		i;
	char	*input;

	i = 0;
	input = shell->input;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (input[i] == '\'')
			i = handle_single_quotes(shell, i, head, tail);
		else if (input[i] == '"')
			i = handle_double_quotes(shell, i, head, tail);
		else if (ft_strchr("|<>", input[i]))
			i = handle_operator(shell, i, head, tail);
		else if (input[i])
			i = handle_word(shell, i, head, tail);
		if (i < 0)
			return (i);
	}
	return (i);
}

int	check_unsupported_characters(char *content)
{
	if (ft_strlen(content) == 1
		&& (!ft_strncmp(content, ";", 1)
			|| !ft_strncmp(content, "\\", 1)
			|| !ft_strncmp(content, "&", 1)))
	{
		return (1);
	}
	return (0);
}

int	check_unsupported_tokens(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp && tmp->content)
	{
		if (check_unsupported_characters(tmp->content))
		{
			return (-4);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	is_syntax_valid(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (0);
	if (tokens->type == T_PIPE || \
		(tokens->type >= T_REDIRECT_IN && tokens->type <= T_APPEND))
		return (0);
	curr = tokens;
	while (curr)
	{
		if (curr->type == T_PIPE && \
			(!curr->next || curr->next->type == T_PIPE))
			return (0);
		if ((curr->type >= T_REDIRECT_IN && curr->type <= T_APPEND) && \
			(!curr->next || curr->next->type != T_WORD))
			return (0);
		curr = curr->next;
	}
	return (1);
}

t_token	*tokenize_input(t_shell *shell)
{
	t_token		*head;
	t_token		*tail;
	int			i;
	int			j;

	head = NULL;
	tail = NULL;
	i = process_tokens(shell, &head, &tail);
	j = check_unsupported_tokens(head);
	if (i < 0 || j != 0 || !is_syntax_valid(head))
	{
		if (j != 0 || !is_syntax_valid(head))
			i = -4;
		print_syntax_error(i);
		free_tokens(head);
		return (NULL);
	}
	shell->tokens = head;
	return (head);
}
