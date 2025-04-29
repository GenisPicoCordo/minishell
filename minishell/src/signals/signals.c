/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/29 15:38:42 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_shell_state = SHELL_NORMAL;

void	handle_sigint(void)
{
	int	state;

	state = signal_flag(GET, 0);
	write(STDERR_FILENO, "\n", 1);
	if (state == SHELL_HEREDOC)
		handle_signal_heredoc_interrupt();
	else if (state == SHELL_CHILD)
		return ;
	else
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
	struct sigaction	act;

	act.sa_handler = handle_signal;
	act.sa_flags = SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
