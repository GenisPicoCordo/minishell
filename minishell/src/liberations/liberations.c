/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   liberations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:33:51 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 11:20:29 by gpico-co         ###   ########.fr       */
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

	if (!table)
		return ;
	i = 0;
	while (i < table->count)
	{
		if (table->cmds[i].args)
			free_split(table->cmds[i].args);
		if (table->cmds[i].cmd)
			free(table->cmds[i].cmd);
		if (table->cmds[i].heredoc_names)
			free_split(table->cmds[i].heredoc_names);
		if (table->cmds[i].redirs)
			free_tokens(table->cmds[i].redirs);
		i++;
	}
	if (table->cmds)
		free(table->cmds);
	free(table);
}
