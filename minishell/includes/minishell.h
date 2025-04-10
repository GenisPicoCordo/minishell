/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:35:52 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 12:00:55 by gpico-co         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*typedef struct s_mishell
{
		env;
		tokens;
		cmd_table;
}*/

//MAIN and LOOP functions

//EXECUTOR
int		count_tokens(t_token *list);
char	**build_argv(t_token *tokens);
void	execute_tokens(t_token *tokens, char **envp, t_env **env_list);

//PARSER
t_token	*create_token_node(char *word, int is_command);
void	free_split(char **split);
t_token	*mock_tokenize_input(char *input);

//BUILTINS
int		is_builtin(char *cmd);
int		execute_builtin(char **argv, t_env **env_list);
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv);
int		builtin_env(char **argv, t_env *env);
int		builtin_exit(char **argv);
int		builtin_cd(char **argv, t_env **env_list);
int		print_export_env(t_env *env);
int		process_single_export(char *arg, t_env **env_list);
int		builtin_export(char **argv, t_env **env_list);
int		builtin_unset(char **argv, t_env **env_list);

//BUILTIN UTILS
int		is_valid_identifier(const char *str);
int		is_numeric(const char *str);
int		str_is_overflowing_long(const char *str);

//ENV
t_env	*create_env_node(char *entry);
t_env	*env_init(char **envp);
int		split_key_value(char *arg, char **key, char **value);
char	*env_get(t_env *env, const char *key);
void	env_set(t_env **env, const char *key, const char *value);
void	env_unset(t_env **env, const char *key);

//UTILS
int		str_is_overflowing_long(const char *str);

//LIBERATIONS
void	free_tokens(t_token *tokens);
void	free_env_list(t_env *env);
void	clean_exit(t_env *env_list, t_token *tokens, \
	char *input, int exit_code);

#endif
