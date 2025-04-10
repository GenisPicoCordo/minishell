/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:10:57 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/10 12:31:49 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_quotes(t_shell *shell, int i, t_token **head, t_token **tail)
{
	int		start;
	t_token	*token;

	start = ++i;
	while (shell->input[i] && shell->input[i] != '\'')
		i++;
	if (!shell->input[i])
		return (SINGLE_QUOTE_UNCLOSED);
	token = create_token(&shell->input[start], i - start, T_WORD);
	append_token(head, tail, token);
	return (i + 1);
}

int	handle_double_quotes(t_shell *shell, int i, t_token **head, t_token **tail)
{
	int		start;
	char	*raw;
	char	*expanded;
	t_token	*token;

	start = ++i;
	while (shell->input[i] && shell->input[i] != '"')
		i++;
	if (!shell->input[i])
		return (DOUBLE_QUOTE_UNCLOSED);
	raw = strndup(&shell->input[start], i - start);
	if (!raw)
		return (-1);
	expanded = expand_string(raw, shell);
	free(raw);
	if (!expanded)
		return (-1);
	token = create_token(expanded, ft_strlen(expanded), T_WORD);
	free(expanded);
	append_token(head, tail, token);
	return (i + 1);
}
