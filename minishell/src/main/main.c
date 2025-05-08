/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/08 12:02:42 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_shell(t_shell *shell)
{
	shell->input = NULL;
	shell->tokens = NULL;
	shell->cmd_table = NULL;
	shell->last_status = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: this program does not take arguments", 2);
		return (1);
	}
	setup_signal_handlers();
	init_shell(&shell);
	shell.env_list = env_init(envp);
	shell.last_status = 0;
	main_loop(&shell);
	return (shell.last_status);
}
