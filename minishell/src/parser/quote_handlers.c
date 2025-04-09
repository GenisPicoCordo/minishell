/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:10:57 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 12:26:15 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// --- Handlers de comillas y palabras ---
int	handle_single_quotes(const char *input, int i, t_lexer_ctx *ctx)
{
	int		start;
	t_token	*token;

	start = ++i;
	while (input[i] && input[i] != '\'')
		i++;
	if (!input[i])
		return (SINGLE_QUOTE_UNCLOSED);
	token = create_token(&input[start], i - start, T_WORD);
	append_token(ctx->head, ctx->tail, token);
	return (i + 1);
}

int	handle_double_quotes(const char *input, int i, t_lexer_ctx *ctx)
{
	int		start;
	t_token	*token;
	char	*raw;
	char	*expanded;

	start = ++i;
	while (input[i] && input[i] != '"')
		i++;
	if (!input[i])
		return (DOUBLE_QUOTE_UNCLOSED);
	raw = strndup(&input[start], i - start);
	expanded = expand_string(raw, ctx->env, ctx->last_status);
	token = create_token(expanded, strlen(expanded), T_WORD);
	free(raw);
	free(expanded);
	append_token(ctx->head, ctx->tail, token);
	return (i + 1);
}
