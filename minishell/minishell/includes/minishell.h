#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>

//static int			g_var;

typedef enum e_type_tokens
{
	T_COMMANDS,
	T_ARGUMENT
}	t_token_type;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

//MAIN and LOOP functions
t_token	*new_token(char *content, t_token_type type);

//EXECUTOR
int		count_tokens(t_token *list);
char	**build_argv(t_token *tokens);
void	execute_tokens(t_token *tokens, char **env);

//LIBERATIONS
void	free_tokens(t_token *tokens);

#endif
