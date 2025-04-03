/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:11:09 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/03 11:11:09 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//Función muy básica que simula el trabajo de Noemi con el parser.

t_token	*create_token_node(char *word, int is_command)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->content = ft_strdup(word);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	if (is_command)
		new->type = T_COMMANDS;
	else
		new->type = T_ARGUMENT;
	new->next = NULL;
	return (new);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

t_token	*mock_tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*current;
	t_token		*new;
	int			i;

	words = ft_split(input, ' ');
	if (!words)
		return (NULL);
	head = NULL;
	current = NULL;
	i = 0;
	while (words[i])
	{
		new = create_token_node(words[i], (i == 0));
		if (!new)
			break ;
		if (!head)
			head = new;
		else
			current->next = new;
		current = new;
		i++;
	}
	free_split(words);
	return (head);
}