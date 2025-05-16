/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:53 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/05/08 13:38:49 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd_table	*parse_tokens(t_token *tokens)
{
	t_cmd_table	*table;
	int			count;
	int			i;

	count = count_pipes(tokens);
	if (!init_cmd_table(&table, count))
		return (NULL);
	fill_cmd_table(table, tokens);
	i = 0;
	while (i < table->count)
	{
		if (!table->cmds[i].args || !table->cmds[i].cmd)
		{
			free_cmd_table(table);
			return (NULL);
		}
		i++;
	}
	return (table);
}
