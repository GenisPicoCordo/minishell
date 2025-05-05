/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:20 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/05 13:59:56 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_word(t_shell *shell, int i, t_token **head, t_token **tail)
{
	int		start;
	char	*raw;
	char	*expanded;
	t_token	*token;

	start = i;
	while (shell->input[i] && shell->input[i] != ' '
		&& !ft_strchr("|<>\"'", shell->input[i]))
		i++;
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

int	process_tokens(t_shell *shell, t_token **head, t_token **tail)
{
	int		i;
	char	*input;

	i = 0;
	input = shell->input;
	while (input[i])
	{
		i = skip_spaces(input, i);
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

// --- Función principal del lexer ---
t_token	*tokenize_input(t_shell *shell)
{
	t_token		*head;
	t_token		*tail;
	int			i;

	head = NULL;
	tail = NULL;
	i = process_tokens(shell, &head, &tail);
	if (i < 0)
	{
		print_quote_error(i);
		free_tokens(head);
		return (NULL);
	}
	shell->tokens = head;
	return (head);
}
