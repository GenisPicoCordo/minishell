/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:08:48 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 14:21:45 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// --- Funciones de utilidad ---
int	skip_spaces(const char *input, int i)
{
	while (input[i] == ' ')
		i++;
	return (i);
}

t_token	*create_token(const char *start, int len, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = strndup(start, len);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, t_token **tail, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}
