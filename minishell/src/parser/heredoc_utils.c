/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:47:00 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 15:47:14 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **get_heredoc_names(t_token *redirs)
{
	int		count = 0;
	int		i = 0;
	t_token	*tmp = redirs;
	char	**names;

	// Contar los heredocs
	while (tmp)
	{
		if (tmp->type == T_HEREDOC && tmp->next && tmp->next->type == T_WORD)
			count++;
		tmp = tmp->next;
	}

	names = malloc(sizeof(char *) * (count + 1));
	if (!names)
		return (NULL);

	tmp = redirs;
	while (tmp)
	{
		if (tmp->type == T_HEREDOC && tmp->next && tmp->next->type == T_WORD)
		{
			names[i++] = ft_strdup(tmp->next->content);
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	names[i] = NULL;
	return names;
}
