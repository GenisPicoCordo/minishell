/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:54:53 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 15:54:11 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Cuenta cuántos pipes hay para saber cuántos comandos existen
static int count_pipes(t_token *tokens)
{
	int count = 1;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}
	return count;
}

// Cuenta cuántos T_WORD hay para reservar espacio en args[]
static int count_args(t_token *tokens)
{
	int count = 0;
	while (tokens && tokens->type != T_PIPE)
	{
		if (tokens->type == T_WORD)
			count++;
		tokens = tokens->next;
	}
	return count;
}

// Duplica una redirección y la añade a la lista de redirs del comando
static void append_redir(t_token **redirs, t_token *redir)
{
	t_token *new = create_token(redir->content, ft_strlen(redir->content), redir->type);
	if (!new)
		return ;
	if (!*redirs)
		*redirs = new;
	else
	{
		t_token *tmp = *redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Llena un comando individual (cmd, args, redirs)
static void fill_command(t_cmd *cmd, t_token *start, t_token *end)
{
	int		arg_count = count_args(start);
	int		arg_index = 0;
	t_token	*tmp = start;

	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return ;

	while (tmp != end && tmp)
	{
		if (tmp->type == T_WORD)
		{
			cmd->args[arg_index++] = ft_strdup(tmp->content);
			if (!cmd->cmd)
				cmd->cmd = ft_strdup(tmp->content);
		}
		else if (tmp->type >= T_REDIRECT_IN && tmp->type <= T_HEREDOC)
		{
			append_redir(&cmd->redirs, tmp);
			if (tmp->next && tmp->next->type == T_WORD)
			{
				tmp = tmp->next;
				append_redir(&cmd->redirs, tmp);
			}
		}
		tmp = tmp->next;
	}
	cmd->args[arg_index] = NULL;

	cmd->heredoc_names = get_heredoc_names(cmd->redirs);
}


// Función principal del parser: convierte tokens en tabla de comandos
t_cmd_table *parse_tokens(t_token *tokens)
{
	t_cmd_table	*table;
	t_token		*start = tokens;
	t_token		*current = tokens;
	int			i = 0;

	table = malloc(sizeof(t_cmd_table));
	if (!table)
		return NULL;
	table->count = count_pipes(tokens);
	table->cmds = malloc(sizeof(t_cmd) * table->count);
	if (!table->cmds)
		return NULL;

	while (i < table->count)
	{
		table->cmds[i].cmd = NULL;
		table->cmds[i].args = NULL;
		table->cmds[i].redirs = NULL;

		while (current && current->type != T_PIPE)
			current = current->next;
        table->cmds[i].index = i;
		fill_command(&table->cmds[i], start, current);
		if (current)
		{
			start = current->next;
			current = current->next;
		}
		i++;
	}
	return table;
}

#include "../../includes/minishell.h"

#include "../../includes/minishell.h"

void	print_cmd_table(t_cmd_table *table)
{
	int			i;
	int			j;
	t_token		*redir;

	if (!table)
	{
		printf("No command table.\n");
		return;
	}

	printf("Parsed command table (%d cmds):\n", table->count);

	i = 0;
	while (i < table->count)
	{
		printf("  ───── Command %d ─────\n", table->cmds[i].index);
		printf("  cmd: %s\n", table->cmds[i].cmd ? table->cmds[i].cmd : "(null)");

		printf("  args: ");
		if (table->cmds[i].args)
		{
			j = 0;
			while (table->cmds[i].args[j])
			{
				printf("[%s] ", table->cmds[i].args[j]);
				j++;
			}
		}
		printf("\n");

		printf("  redirs: ");
		redir = table->cmds[i].redirs;
		while (redir)
		{
			printf("[%s] ", redir->content);
			redir = redir->next;
		}
		printf("\n");

		printf("  heredoc_names: ");
		if (table->cmds[i].heredoc_names)
		{
			j = 0;
			while (table->cmds[i].heredoc_names[j])
			{
				printf("[%s] ", table->cmds[i].heredoc_names[j]);
				j++;
			}
		}
		printf("\n\n");
		i++;
	}
}
