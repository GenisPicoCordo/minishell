/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:52:39 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/30 15:22:18 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_heredoc_interrupt(int fd, char *line, char *tmp_name)
{
	if (line)
		free(line);
	close(fd);
	unlink(tmp_name);
	free(tmp_name);
	signal_flag(SET, SHELL_HEREDOC_INTERRUPTED);
	return (1);
}

int	write_heredoc_lines(int fd, const char *delimiter, char *tmp_name)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (signal_flag(GET, 0) == SHELL_HEREDOC_INTERRUPTED)
			return (handle_heredoc_interrupt(fd, line, tmp_name));
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

char	*generate_tmp_filename(void)
{
	static int	id = 0;
	char		*number;
	char		*filename;

	number = ft_itoa(id++);
	if (!number)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", number);
	free(number);
	return (filename);
}
