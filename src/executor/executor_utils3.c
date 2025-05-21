/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:58:28 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/05 15:49:48 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**build_argv(t_token *tokens)
{
	int		i;
	int		count;
	char	**argv;

	i = 0;
	count = count_tokens(tokens);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == T_COMMANDS || tokens->type == T_ARGUMENT
			|| tokens->type == T_WORD)
			argv[i++] = tokens->content;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	count_tokens(t_token *list)
{
	int	count;

	count = 0;
	while (list)
	{
		if (list->type == T_COMMANDS || list->type == T_ARGUMENT
			|| list->type == T_WORD)
			count++;
		list = list->next;
	}
	return (count);
}
