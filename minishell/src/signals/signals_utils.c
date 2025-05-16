/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:23:51 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/16 13:09:11 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	signal_flag(int mode, int val)
{
	if (mode == SET)
		g_shell_state = val;
	return (g_shell_state);
}

void	handle_signal_heredoc_interrupt(void)
{
	char	dummy;

	dummy = ' ';
	signal_flag(SET, SHELL_HEREDOC_INTERRUPTED);
	rl_done = 1;
	ioctl(STDIN_FILENO, TIOCSTI, &dummy);
}

void	handle_normal_interrupt(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	signal_flag(SET, SHELL_NORMAL);
}

void	handle_child_interrupt(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
	signal_flag(SET, SHELL_INTERRUPTED);
}
