/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncampo-f <ncampo-f@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:46:39 by ncampo-f          #+#    #+#             */
/*   Updated: 2025/04/09 15:56:21 by ncampo-f         ###   ########.fr       */
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

typedef struct s_lexer_ctx
{
	char		**env;
	int			last_status;
	t_token		**head;
	t_token		**tail;
}	t_lexer_ctx;

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

//MAIN and LOOP functions

//EXECUTOR
int		count_tokens(t_token *list);
char	**build_argv(t_token *tokens);
int		execute_tokens(t_token *tokens, char **env);
//void	execute_tokens(t_token *tokens, char **env);

//PARSER

t_token	*create_token_node(char *word, int is_command);
void	free_split(char **split);
t_token	*mock_tokenize_input(char *input);
t_token	*tokenize_input(const char *input, char **env, int last_status);
int		process_tokens(const char *input, t_lexer_ctx *ctx);

// ──────────────────────────────────────────────────────────────
// LEXER: UTILS
// ──────────────────────────────────────────────────────────────

int		skip_spaces(const char *input, int i);
t_token	*create_token(const char *start, int len, t_token_type type);
void	append_token(t_token **head, t_token **tail, t_token *new);

// ──────────────────────────────────────────────────────────────
// LEXER: HANDLERS
// ──────────────────────────────────────────────────────────────

int		handle_word(const char *input, int i, t_lexer_ctx *ctx);
int		handle_single_quotes(const char *input, int i, t_lexer_ctx *ctx);
int		handle_double_quotes(const char *input, int i, t_lexer_ctx *ctx);
int		handle_operator(const char *input, int i, t_lexer_ctx *ctx);

// ──────────────────────────────────────────────────────────────
// LEXER: OPERATORS
// ──────────────────────────────────────────────────────────────

t_token	*handle_logical_ops(const char *input, int *i);
t_token	*handle_pipe(const char *input, int *i);
t_token	*handle_redirect(const char *input, int *i);

// ──────────────────────────────────────────────────────────────
// LEXER: EXPANSION
// ──────────────────────────────────────────────────────────────

char	*expand_variable(const char *str, int *i, char **env, int last_status);
char	*expand_string(const char *input, char **env, int last_status);

// ──────────────────────────────────────────────────────────────
// LEXER: ERRORES
// ──────────────────────────────────────────────────────────────

void	print_quote_error(t_quote_error err);

t_cmd_table	*parse_tokens(t_token *tokens);

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
char	*find_command_path(char *cmd);

//LIBERATIONS
void	free_tokens(t_token *tokens);
void	free_cmd_table(t_cmd_table *table);

void	print_cmd_table(t_cmd_table *table);
int		validate_cmd_table(t_cmd_table *table, char **env);
char	**get_heredoc_names(t_token *redirs);

#endif
