#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>
# include <errno.h>

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

//EXECUTOR
int		count_tokens(t_token *list);
char	**build_argv(t_token *tokens);
void	execute_tokens(t_token *tokens, char **env);

//PARSER
t_token	*create_token_node(char *word, int is_command);
void	free_split(char **split);
t_token	*mock_tokenize_input(char *input);

//BUILTINS
int		is_builtin(char *cmd);
int		execute_builtin(char **argv, char **env);
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv);
int		builtin_env(char **argv, char **env);
int		is_numeric(const char *str);
int		builtin_exit(char **argv);
int		validate_cd_args(char **argv);
char	*get_cd_path(char **argv);
int		builtin_cd(char **argv);

//UTILS
int		str_is_overflowing_long(const char *str);

//LIBERATIONS
void	free_tokens(t_token *tokens);

#endif
