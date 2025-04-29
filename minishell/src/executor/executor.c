/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:55:00 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 14:57:35 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_for_all(void)
{
	while (wait(NULL) > 0)
		;
}

void	handle_pipe_and_fork(t_cmd_table *table, t_env **env_list, \
	t_pipeinfo *info)
{
	pid_t	pid;

	if (info->i < table->count - 1)
	{
		if (pipe(info->pipefd) == -1)
			perror("pipe");
	}
	pid = fork();
	if (pid == 0)
		child_setup(table, env_list, info);
	else if (pid < 0)
		perror("fork");
}

void	cleanup_parent_fds(t_pipeinfo *info, int total)
{
	if (info->i != 0)
		close(info->in_fd);
	if (info->i < total - 1)
	{
		close(info->pipefd[1]);
		info->in_fd = info->pipefd[0];
	}
}

void	loop_pipeline(t_cmd_table *table, t_env **env_list, t_pipeinfo *info)
{
	while (info->i < table->count)
	{
		if (!table->cmds[info->i].cmd)
		{
			info->i++;
			continue ;
		}
		handle_pipe_and_fork(table, env_list, info);
		cleanup_parent_fds(info, table->count);
		info->i++;
	}
}

int	execute_pipeline(t_cmd_table *table, t_env **env_list)
{
	t_pipeinfo	info;

	info.i = 0;
	info.in_fd = 0;
	loop_pipeline(table, env_list, &info);
	wait_for_all();
	return (0);
}
