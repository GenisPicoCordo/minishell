/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:29:27 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/01 15:47:02 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *content, t_token_type type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	tok->content = content;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break ;
		}
		//aqui en la 49 ira el parser real, tokens = parser(input);
		tokens = new_token("/bin/ls", T_COMMANDS);
		tokens->next = new_token("-l", T_ARGUMENT);
		tokens->next->next = new_token("-a", T_ARGUMENT);
		execute_tokens(tokens, envp);
		free_tokens(tokens);
		free(input);
	}
}
