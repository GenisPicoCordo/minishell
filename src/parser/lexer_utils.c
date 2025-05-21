/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:08:48 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/19 17:44:49 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_word_with_quotes(t_shell *shell, int i)
{
	while (shell->input[i] && shell->input[i] != ' '
		&& !ft_strchr("|<>\"'", shell->input[i]))
	{
		if (shell->input[i] == '=' && shell->input[i + 1] == '"')
		{
			i += 2;
			while (shell->input[i] && shell->input[i] != '"')
				i++;
			if (shell->input[i])
				i++;
		}
		else if (shell->input[i] == '=' && shell->input[i + 1] == '\'')
		{
			i += 2;
			while (shell->input[i] && shell->input[i] != '\'')
				i++;
			if (shell->input[i])
				i++;
		}
		else
			i++;
	}
	return (i);
}

int	handle_word(t_shell *shell, int i, t_token **head, t_token **tail)
{
	int		start;
	char	*raw;
	char	*expanded;
	t_token	*token;

	start = i;
	i = skip_word_with_quotes(shell, i);
	raw = ft_strndup(&shell->input[start], i - start);
	if (!raw)
		return (-1);
	expanded = expand_string(raw, shell);
	free(raw);
	if (!expanded)
		return (-1);
	token = create_token(expanded, ft_strlen(expanded), T_WORD);
	free(expanded);
	append_token(head, tail, token);
	return (i);
}

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
	token->content = ft_strndup(start, len);
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
