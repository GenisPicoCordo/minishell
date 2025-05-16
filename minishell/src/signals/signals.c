/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/16 12:59:51 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_shell_state = SHELL_NORMAL;

void	handle_sigint(void)
{
	int	state;

	state = signal_flag(GET, 0);
	if (state == SHELL_HEREDOC)
	{
		write(STDERR_FILENO, "\n", 1);
		handle_signal_heredoc_interrupt();
	}
	else if (state == SHELL_CHILD)
		handle_child_interrupt();
	else if (state == SHELL_NORMAL)
		handle_normal_interrupt();
}

void	handle_sigquit(void)
{
	if (signal_flag(GET, 0) == SHELL_CHILD)
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
}

void	handle_signal(int signo)
{
	if (signo == SIGINT)
		handle_sigint();
	else if (signo == SIGQUIT)
		handle_sigquit();
}

void	setup_signal_handlers(void)
{
	struct sigaction	act_int;
	struct sigaction	act_quit;

	act_int.sa_handler = handle_signal;
	act_int.sa_flags = SA_RESTART;
	sigemptyset(&act_int.sa_mask);
	act_quit.sa_flags = SA_RESTART;
	act_quit.sa_handler = SIG_IGN;
	sigemptyset(&act_quit.sa_mask);
	sigaction(SIGINT, &act_int, NULL);
	sigaction(SIGQUIT, &act_quit, NULL);
	signal(SIGTSTP, SIG_IGN);
}
