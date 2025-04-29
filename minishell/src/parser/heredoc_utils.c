/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:47:00 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/29 17:34:36 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_heredocs(t_token *redirs)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = redirs;
	while (tmp)
	{
		if (tmp->type == T_HEREDOC
			&& tmp->next
			&& tmp->next->type == T_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static void	fill_heredoc_names(t_token *redirs, char **names)
{
	t_token	*tmp;
	int		i;

	tmp = redirs;
	i = 0;
	while (tmp)
	{
		if (tmp->type == T_HEREDOC
			&& tmp->next
			&& tmp->next->type == T_WORD)
		{
			names[i] = ft_strdup(tmp->next->content);
			i++;
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	names[i] = NULL;
}

char	**get_heredoc_names(t_token *redirs)
{
	int		count;
	char	**names;

	count = count_heredocs(redirs);
	names = malloc(sizeof(char *) * (count + 1));
	if (!names)
		return (NULL);
	fill_heredoc_names(redirs, names);
	return (names);
}
