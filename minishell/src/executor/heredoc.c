/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:11:02 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	preprocess_heredocs(t_cmd_table *table)
{
	int		i;
	t_token	*redir;
	char	*tmp_path;

	i = 0;
	while (i < table->count)
	{
		redir = table->cmds[i].redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC && redir->next)
			{
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
			}
			redir = redir->next;
		}
		i++;
	}
	signal_flag(SET, SHELL_NORMAL);
	return (0);
}

char	*generate_tmp_filename(void)
{
	static int	id = 0;
	char		*number;
	char		*filename;

	number = ft_itoa(id++);
	filename = ft_strjoin("/tmp/minishell_heredoc_", number);
	free(number);
	return (filename);
}

char *create_heredoc_file(const char *delimiter)
{
	char	*line;
	char	*tmp_name = generate_tmp_filename();
	int		fd;

	signal_flag(SET, SHELL_HEREDOC);
	fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (perror("heredoc open"), free(tmp_name), NULL);

	while (1)
	{
		line = readline("> ");
		if (signal_flag(GET, 0) == SHELL_HEREDOC_INTERRUPTED)
		{
			if (line)
				free(line);
			close(fd);
			unlink(tmp_name);
			free(tmp_name);
			signal_flag(SET, SHELL_NORMAL);
			return (NULL);
		}
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	signal_flag(SET, SHELL_NORMAL);
	return (tmp_name);
}


