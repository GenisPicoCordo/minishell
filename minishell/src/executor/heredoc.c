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
				tmp_path = create_heredoc_file(redir->next->content);
				if (!tmp_path)
					return (1);
				// Cambiamos el tipo del redir a T_REDIRECT_IN y el contenido al path
				redir->type = T_REDIRECT_IN;
				free(redir->next->content);
				redir->next->content = tmp_path;
			}
			redir = redir->next;
		}
		i++;
	}
	return (0);
}

char *generate_tmp_filename(void)
{
	static int	id = 0;
	char		*number;
	char		*filename;
	char		*prefix = "/tmp/minishell_heredoc_";

	number = ft_itoa(id++);
	filename = ft_strjoin(prefix, number);
	free(number);
	return (filename);
}

char *create_heredoc_file(const char *delimiter)
{
	char	*line;
	int		fd;
	char	*tmp_name = generate_tmp_filename();

	fd = open(tmp_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("heredoc open");
		free(tmp_name);
		return (NULL);
	}

	while (1)
    {
	    line = readline("> ");
	    if (!line || (ft_strlen(line) == ft_strlen(delimiter)
		    && ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0))
		    break;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
	return (tmp_name); // No strdup: ya es dinámico
}

