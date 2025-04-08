/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:20 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/08 16:53:26 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>
#include <string.h>

// --- Tipos y estructuras auxiliares ---
typedef enum e_quote_error
{
	NO_ERROR = 0,
	SINGLE_QUOTE_UNCLOSED = -1,
	DOUBLE_QUOTE_UNCLOSED = -2,
	INVALID_OPERATOR = -5
}	t_quote_error;

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

// --- Expansión de variables ---
char	*expand_variable(const char *str, int *i, __attribute__((unused)) char **env, int last_status)
{
	char	var[256];
	int		j;
	char	*value;

	j = 0;
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(last_status));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var[j++] = str[(*i)++];
	var[j] = '\0';
	value = getenv(var);
	if (value)
		return (ft_strdup(value));
	else
		return (strdup(""));
}

char	*expand_string(const char *input, char **env, int last_status)
{
	char	*result;
	char	*fragment;
	int		i;

	result = strdup("");
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			char *exp = expand_variable(input, &i, env, last_status);
			char *tmp = ft_strjoin(result, exp);
			free(exp);
			free(result);
			result = tmp;
		}
		else
		{
			int start = i;
			while (input[i] && input[i] != '$')
				i++;
			fragment = strndup(&input[start], i - start);
			char *tmp = ft_strjoin(result, fragment);
			free(fragment);
			free(result);
			result = tmp;
		}
	}
	return result;
}

// --- Handlers de comillas y palabras ---
int handle_single_quotes(const char *input, int i, t_token **head, t_token **tail)
{
	int		start;
	t_token	*token;

	start = ++i;
	while (input[i] && input[i] != '\'')
		i++;
	if (!input[i])
		return (SINGLE_QUOTE_UNCLOSED);
	token = create_token(&input[start], i - start, T_WORD);
	append_token(head, tail, token);
	return (i + 1);
}

int handle_double_quotes(const char *input, int i, t_token **head, t_token **tail, char **env, int last_status)
{
	int		start;
	t_token	*token;

	start = ++i;
	while (input[i] && input[i] != '"')
		i++;
	if (!input[i])
		return DOUBLE_QUOTE_UNCLOSED;
	char *raw = strndup(&input[start], i - start);
	char *expanded = expand_string(raw, env, last_status);
	token = create_token(expanded, strlen(expanded), T_WORD);
	free(raw);
	free(expanded);
	append_token(head, tail, token);
	return i + 1;
}

int handle_word(const char *input, int i, t_token **head, t_token **tail, char **env, int last_status)
{
	int start = i;
	while (input[i] && input[i] != ' ' && !ft_strchr("|<>\"'", input[i]))
		i++;
	char *raw = strndup(&input[start], i - start);
	char *expanded = expand_string(raw, env, last_status);
	t_token *token = create_token(expanded, strlen(expanded), T_WORD);
	free(raw);
	free(expanded);
	append_token(head, tail, token);
	return i;
}

int handle_operator(const char *input, int i, t_token **head, t_token **tail)
{
	t_token *token = NULL;
	if (input[i] == '|' && input[i + 1] == '|')
		return INVALID_OPERATOR;
	else if (input[i] == '|')
		token = create_token("|", 1, T_PIPE);
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		token = create_token("<<", 2, T_HEREDOC);
		i++;
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		token = create_token(">>", 2, T_APPEND);
		i++;
	}
	else if (input[i] == '<')
		token = create_token("<", 1, T_REDIRECT_IN);
	else if (input[i] == '>')
		token = create_token(">", 1, T_REDIRECT_OUT);
	else
		return -4;
	append_token(head, tail, token);
	return i + 1;
}

// --- Función principal del lexer ---
t_token	*tokenize_input(const char *input, char **env, int last_status)
{
	t_token *head = NULL;
	t_token *tail = NULL;
	int i = 0;

	while (input[i])
	{
		i = skip_spaces(input, i);
		if (input[i] == '\'')
			i = handle_single_quotes(input, i, &head, &tail);
		else if (input[i] == '"')
			i = handle_double_quotes(input, i, &head, &tail, env, last_status);
		else if (ft_strchr("|<>", input[i]))
			i = handle_operator(input, i, &head, &tail);
		else if (input[i])
			i = handle_word(input, i, &head, &tail, env, last_status);
		if (i == SINGLE_QUOTE_UNCLOSED)
			return (ft_putendl_fd("Syntax error: single quote not closed", 2), free_tokens(head), NULL);
		if (i == DOUBLE_QUOTE_UNCLOSED)
			return (ft_putendl_fd("Syntax error: double quote not closed", 2), free_tokens(head), NULL);
		if (i == INVALID_OPERATOR)
			return (ft_putendl_fd("Syntax error: unexpected token `||`", 2), free_tokens(head), NULL);
	}
	return head;
}
