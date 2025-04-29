/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:02:06 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 14:53:33 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_heredoc_file(const char *delimiter)
{
	char	*tmp_name;
	int		fd;

	tmp_name = generate_tmp_filename();
	if (!tmp_name)
		return (NULL);
	fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		free(tmp_name);
		return (NULL);
	}
	signal_flag(SET, SHELL_HEREDOC);
	if (write_heredoc_lines(fd, delimiter, tmp_name))
		return (NULL);
	close(fd);
	signal_flag(SET, SHELL_NORMAL);
	return (tmp_name);
}

int	handle_heredoc_token(t_token *redir)
{
	char	*tmp_path;

	signal_flag(SET, SHELL_HEREDOC);
	tmp_path = create_heredoc_file(redir->next->content);
	if (signal_flag(GET, 0) == SHELL_HEREDOC_INTERRUPTED || !tmp_path)
	{
		signal_flag(SET, SHELL_NORMAL);
		return (1);
	}
	redir->type = T_REDIRECT_IN;
	free(redir->next->content);
	redir->next->content = tmp_path;
	return (0);
}

int	preprocess_heredocs(t_cmd_table *table)
{
	int		i;
	t_token	*redir;

	i = 0;
	while (i < table->count)
	{
		redir = table->cmds[i].redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC && redir->next)
				if (handle_heredoc_token(redir))
					return (1);
			redir = redir->next;
		}
		i++;
	}
	signal_flag(SET, SHELL_NORMAL);
	return (0);
}
