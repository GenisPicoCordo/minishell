/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 14:51:57 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_input_redir(t_token *redir)
{
	int	fd;

	fd = open(redir->next->content, O_RDONLY);
	if (fd < 0)
		return (perror("open <"), 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_output_redir(t_token *redir)
{
	int	fd;

	fd = open(redir->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("open >"), 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append_redir(t_token *redir)
{
	int	fd;

	fd = open(redir->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("open >>"), 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_heredoc_redir(t_token *redir)
{
	int		fd;
	char	*tmp_path;

	tmp_path = create_heredoc_file(redir->next->content);
	if (!tmp_path)
		return (1);
	fd = open(tmp_path, O_RDONLY);
	free(tmp_path);
	if (fd < 0)
		return (perror("open heredoc"), 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	apply_redirections(t_token *redir)
{
	while (redir && redir->next)
	{
		if (redir->type == T_REDIRECT_IN && handle_input_redir(redir))
			return (1);
		else if (redir->type == T_REDIRECT_OUT && handle_output_redir(redir))
			return (1);
		else if (redir->type == T_APPEND && handle_append_redir(redir))
			return (1);
		else if (redir->type == T_HEREDOC && handle_heredoc_redir(redir))
			return (1);
		redir = redir->next->next;
	}
	return (0);
}
