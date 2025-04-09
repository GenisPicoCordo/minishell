/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:20 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 14:10:20 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_word(const char *input, int i, t_lexer_ctx *ctx)
{
	int		start;
	char	*raw;
	char	*expanded;
	t_token	*token;

	start = i;
	while (input[i] && input[i] != ' ' && !ft_strchr("|<>\"'", input[i]))
		i++;
	raw = strndup(&input[start], i - start);
	expanded = expand_string(raw, ctx->env, ctx->last_status);
	token = create_token(expanded, strlen(expanded), T_WORD);
	free(raw);
	free(expanded);
	append_token(ctx->head, ctx->tail, token);
	return (i);
}

int	process_tokens(const char *input, t_lexer_ctx *ctx)
{
	int	i;

	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (input[i] == '\'')
			i = handle_single_quotes(input, i, ctx);
		else if (input[i] == '"')
			i = handle_double_quotes(input, i, ctx);
		else if (ft_strchr("|<>", input[i]))
			i = handle_operator(input, i, ctx);
		else if (input[i])
			i = handle_word(input, i, ctx);
		if (i < 0)
			return (i);
	}
	return (i);
}

// --- Función principal del lexer ---
t_token	*tokenize_input(const char *input, char **env, int last_status)
{
	t_token		*head;
	t_token		*tail;
	t_lexer_ctx	ctx;
	int			i;

	head = NULL;
	tail = NULL;
	ctx.env = env;
	ctx.last_status = last_status;
	ctx.head = &head;
	ctx.tail = &tail;
	i = process_tokens(input, &ctx);
	if (i < 0)
	{
		print_quote_error(i);
		free_tokens(head);
		return (NULL);
	}
	return (head);
}
