/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   liberations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:33:51 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 15:56:58 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->content);
		free(tokens);
		tokens = tmp;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	clean_exit(t_env *env_list, t_token *tokens, char *input, int exit_code)
{
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
	if (env_list)
		free_env_list(env_list);
	exit(exit_code);
}

void	free_cmd_table(t_cmd_table *table)
{
	int	i;

	i = 0;
	while (i < table->count)
	{
		free_split(table->cmds[i].args);
		free(table->cmds[i].cmd);
		free_split(table->cmds[i].heredoc_names);
		free_tokens(table->cmds[i].redirs);
		i++;
	}
	free(table->cmds);
	free(table);
}
