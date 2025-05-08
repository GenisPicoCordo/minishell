/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:30:20 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/08 14:05:09 by gpico-co         ###   ########.fr       */
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
	int		ret;
	char	*input;

	i = 0;
	input = shell->input;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (input[i] == '\'')
			ret = handle_single_quotes(shell, i, head, tail);
		else if (input[i] == '"')
			ret = handle_double_quotes(shell, i, head, tail);
		else if (ft_strchr("|<>", input[i]))
			ret = handle_operator(shell, i, head, tail);
		else
			ret = handle_word(shell, i, head, tail);
		if (ret < 0)
		{
			free_tokens(*head);
			return (ret);
		}
		i = ret;
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

// --- Función principal del lexer ---
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
	if (i < 0 || j != 0)
	{
		if (j != 0)
			i = -4;
		print_quote_error(i);
		free_tokens(head);
		return (NULL);
	}
	shell->tokens = head;
	return (head);
}
