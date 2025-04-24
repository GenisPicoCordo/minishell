/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                         :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:46:54 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:11:02 by ncampo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	g_shell_state = SHELL_NORMAL;

int	signal_flag(int mode, int val)
{
	if (mode == SET)
		g_shell_state = val;
	return (g_shell_state);
}

void	handle_sigint(void)
{
	int	state = signal_flag(GET, 0);
	write(STDERR_FILENO, "\n", 1);

	if (state == SHELL_HEREDOC)
	{
		signal_flag(SET, SHELL_HEREDOC_INTERRUPTED);
		rl_done = 1;
		// Forzamos simulación de Enter para salir de readline automáticamente
		char dummy = '\n';
		ioctl(STDIN_FILENO, TIOCSTI, &dummy);
	}
	else if (state == SHELL_CHILD)
		return ;
	else
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
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