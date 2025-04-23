/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:11:02 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int apply_redirections(t_token *redirs)
{
	int		fd;
	char	*tmp_path;

	while (redirs)
	{
		if (redirs->type == T_REDIRECT_IN)
		{
			if (!redirs->next)
				return (1);
			fd = open(redirs->next->content, O_RDONLY);
			if (fd < 0)
				return (perror("open <"), 1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			redirs = redirs->next;
		}
		else if (redirs->type == T_REDIRECT_OUT)
		{
			if (!redirs->next)
				return (1);
			fd = open(redirs->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (perror("open >"), 1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			redirs = redirs->next;
		}
		else if (redirs->type == T_APPEND)
		{
			if (!redirs->next)
				return (1);
			fd = open(redirs->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (perror("open >>"), 1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			redirs = redirs->next;
		}
		else if (redirs->type == T_HEREDOC)
		{
			if (!redirs->next)
				return (1);
			tmp_path = create_heredoc_file(redirs->next->content);
			if (!tmp_path)
				return (1);
			fd = open(tmp_path, O_RDONLY);
			free(tmp_path);
			if (fd < 0)
				return (perror("open heredoc"), 1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			redirs = redirs->next;
		}
		redirs = redirs->next;
	}
	return (0);
}

