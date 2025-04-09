/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:45 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_env	*env_list;

	(void)argc;
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: this program does not take arguments", 2);
		return (1);
	}
	env_list = env_init(envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			clean_exit(env_list, NULL, NULL, 0);
			break ;
		}
		if (*input)
			add_history(input);
		tokens = mock_tokenize_input(input);
		execute_tokens(tokens, envp, &env_list);
		free_tokens(tokens);
		free(input);
	}
}
