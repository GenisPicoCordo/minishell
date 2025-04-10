/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:35:52 by gpico-co          #+#    #+#             */
/*   Updated: 2025/04/10 15:12:27 by ncampo-f         ###   ########.fr       */
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

//ERROR messages
# define ERR_SINGLE_QUOTE "Syntax error: single quote not closed"
# define ERR_DOUBLE_QUOTE "Syntax error: double quote not closed"
# define ERR_INV_OPERATOR "Syntax error"
# define ERR_UNKNOWN_QUOTE "Unknown quote error"

//static int			g_var;

typedef enum e_type_tokens
{
	T_WORD,
	T_PIPE,
	T_OR,
	T_AND,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC,
	T_COMMANDS,
	T_ARGUMENT
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_quote_error
{
	NO_ERROR = 0,
	SINGLE_QUOTE_UNCLOSED = -1,
	DOUBLE_QUOTE_UNCLOSED = -2,
	INVALID_OPERATOR = -5
}	t_quote_error;

typedef struct s_token
{
	char			*content;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_error_entry
{
	int		code;
	char	*message;
}	t_error_entry;

typedef struct s_cmd
{
	int			index;
	char		*cmd;
	char		**args;
	t_token		*redirs;
	char		**heredoc_names;
}	t_cmd;

typedef struct s_cmd_table
{
	t_cmd	*cmds;
	int		count;
}	t_cmd_table;

typedef struct s_shell
{
	int			last_status;	// último código de retorno
	char		**env;			// entorno
	t_token		*tokens;		// tokens actuales
	t_cmd_table	*cmd_table;		// comandos parseados
	char		*input;			// línea original del input
}	t_shell;

//MAIN and LOOP functions

//EXECUTOR
int		count_tokens(t_token *list);
char	**build_argv(t_token *tokens);
int		execute_tokens(t_token *tokens, t_env **env);

//PARSER

t_token	*create_token_node(char *word, int is_command);
void	free_split(char **split);
t_token	*mock_tokenize_input(char *input);
t_token	*tokenize_input(t_shell *shell);
int		process_tokens(t_shell *shell, t_token **head, t_token **tail);

// ──────────────────────────────────────────────────────────────
// LEXER: UTILS
// ──────────────────────────────────────────────────────────────

int		skip_spaces(const char *input, int i);
t_token	*create_token(const char *start, int len, t_token_type type);
void	append_token(t_token **head, t_token **tail, t_token *new);

// ──────────────────────────────────────────────────────────────
// LEXER: HANDLERS
// ──────────────────────────────────────────────────────────────

int		handle_word(t_shell *shell, int i, t_token **head, t_token **tail);
int		handle_single_quotes(t_shell *shell, int i, t_token **head, t_token **tail);
int		handle_double_quotes(t_shell *shell, int i, t_token **head, t_token **tail);
int		handle_operator(t_shell *shell, int i, t_token **head, t_token **tail);

// ──────────────────────────────────────────────────────────────
// LEXER: OPERATORS
// ──────────────────────────────────────────────────────────────

t_token	*handle_logical_ops(const char *input, int *i);
t_token	*handle_pipe(const char *input, int *i);
t_token	*handle_redirect(const char *input, int *i);

// ──────────────────────────────────────────────────────────────
// LEXER: EXPANSION
// ──────────────────────────────────────────────────────────────

char	*expand_string(const char *input, t_shell *shell);
char	*expand_loop(const char *input, t_shell *shell, char *result);
char	*expand_variable(const char *str, int *i, t_shell *shell);
char	*expand_home(const char *input);

// ──────────────────────────────────────────────────────────────
// LEXER: ERRORES
// ──────────────────────────────────────────────────────────────

void	print_quote_error(t_quote_error err);

t_cmd_table	*parse_tokens(t_token *tokens);

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

//UTILS
int		is_valid_identifier(const char *str);
int		is_numeric(const char *str);
int		str_is_overflowing_long(const char *str);
char	*join_path_cmd(char *path, char *cmd);
char	*search_in_paths(char **paths, char *cmd);
char	*find_command_path(char *cmd);

//ENV
t_env	*create_env_node(char *entry);
t_env	*env_init(char **envp);
int		split_key_value(char *arg, char **key, char **value);
void	env_set(t_env **env, const char *key, const char *value);
void	env_unset(t_env **env, const char *key);
int		env_list_size(t_env *env);
char	**env_to_array(t_env *env);

//LIBERATIONS
void	free_tokens(t_token *tokens);
void	free_cmd_table(t_cmd_table *table);
void	free_env_list(t_env *env);
void	clean_exit(t_env *env_list, t_token *tokens, \
	char *input, int exit_code);

void	print_cmd_table(t_cmd_table *table);
int		validate_cmd_table(t_shell *shell);
char	**get_heredoc_names(t_token *redirs);

#endif
