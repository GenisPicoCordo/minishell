/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpico-co <gpico-co@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:35:52 by gpico-co          #+#    #+#             */
/*   Updated: 2025/05/16 12:50:09 by gpico-co         ###   ########.fr       */
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
# include <sys/ioctl.h> 
# include <errno.h>
# include <fcntl.h>

//ERROR messages
# define ERR_SINGLE_QUOTE "Syntax error: single quote not closed"
# define ERR_DOUBLE_QUOTE "Syntax error: double quote not closed"
# define ERR_INV_OPERATOR "Syntax error"
# define ERR_UNKNOWN_QUOTE "Unknown quote error"
# define ERR_NON_SUPPORTED_TOKEN "Unsuported token detected"

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
	NON_SUPPORTED_TOKEN = -4,
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
	t_cmd		*cmds;
	int			count;
}	t_cmd_table;

typedef struct s_pipeinfo
{
	int		i;
	int		in_fd;
	int		pipefd[2];
}	t_pipeinfo;

typedef struct s_shell
{
	int			last_status;
	t_env		*env_list;
	t_token		*tokens;
	t_cmd_table	*cmd_table;
	char		*input;
}	t_shell;

typedef enum e_shell_state
{
	SHELL_NORMAL = 0,
	SHELL_CHILD = 1,
	SHELL_HEREDOC = 2,
	SHELL_HEREDOC_INTERRUPTED = 3,
	SHELL_INTERRUPTED = 4,
	SHELL_NORMAL_INTERRUPTED = 5,
	SHELL_CTRL_C = 6
}	t_shell_state;

# define GET 0
# define SET 1

extern int	g_shell_state;

//MAIN and LOOP functions
void			exit_if_eof(t_shell *shell);
int				free_and_interrupt(t_shell *shell);
int				handle_interrupt_and_errors(t_shell *shell);
void			main_loop(t_shell *shell);
int				should_skip_execution(t_shell *shell);
void			cleanup_after_execution(t_shell *shell);
int				execute_command_loop(t_shell *shell);

//EXECUTOR

void			wait_for_all(void);
void			handle_pipe_and_fork(t_cmd_table *table, t_env **env_list, \
					t_pipeinfo *info);
void			cleanup_parent_fds(t_pipeinfo *info, int total);
void			loop_pipeline(t_cmd_table *table, t_env **env_list, \
					t_pipeinfo *info);
int				execute_pipeline(t_cmd_table *table, t_env **env_list);
int				exec_external(t_shell *shell, t_cmd *cmd, t_env **env_list);
int				exec_parent_builtin(t_cmd *cmd, t_env **env_list);
int				execute_tokens(t_shell *shell, t_cmd_table *table, \
					t_env **env_list);
void			setup_redirs(t_cmd_table *t, t_pipeinfo *info);
void			exec_builtin_or_external(t_cmd_table *t, t_env **env, \
					t_pipeinfo *info);
void			child_setup(t_cmd_table *t, t_env **env, t_pipeinfo *info);
char			**build_argv(t_token *tokens);
int				is_parent_builtin(char *cmd);
int				exec_builtin_in_child(t_cmd *cmd, t_env **env_list);
int				exec_builtin_in_child(t_cmd *cmd, t_env **env_list);
int				wait_and_return_status(t_shell *shell, int pid, char *cmd_path, \
				char **envp);
void			execve_child_process(t_cmd *cmd, char *cmd_path, char **envp);
int				count_tokens(t_token *list);
char			*create_heredoc_file(const char *delimiter);
int				handle_heredoc_token(t_token *redir);
int				preprocess_heredocs(t_cmd_table *table);
int				handle_heredoc_interrupt(int fd, char *line, char *tmp_name);
int				write_heredoc_lines(int fd, const char *delimiter, \
					char *tmp_name);
char			*generate_tmp_filename(void);
int				handle_input_redir(t_token *redir);
int				handle_output_redir(t_token *redir);
int				handle_append_redir(t_token *redir);
int				handle_heredoc_redir(t_token *redir);
int				apply_redirections(t_token *redir);

//PARSER

t_token			*create_token_node(char *word, int is_command);
void			free_split(char **split);
t_token			*mock_tokenize_input(char *input);
t_token			*tokenize_input(t_shell *shell);
int				process_tokens(t_shell *shell, t_token **head, t_token **tail);

// ──────────────────────────────────────────────────────────────
// LEXER: UTILS
// ──────────────────────────────────────────────────────────────

int				skip_spaces(const char *input, int i);
t_token			*create_token(const char *start, int len, t_token_type type);
void			append_token(t_token **head, t_token **tail, t_token *new);

// ──────────────────────────────────────────────────────────────
// LEXER: HANDLERS
// ──────────────────────────────────────────────────────────────

int				handle_word(t_shell *shell, int i, t_token **head, \
					t_token **tail);
int				handle_single_quotes(t_shell *shell, int i, t_token **head, \
					t_token **tail);
int				handle_double_quotes(t_shell *shell, int i, t_token **head, \
					t_token **tail);
int				handle_operator(t_shell *shell, int i, t_token **head, \
					t_token **tail);

// ──────────────────────────────────────────────────────────────
// LEXER: OPERATORS
// ──────────────────────────────────────────────────────────────

t_token			*handle_logical_ops(const char *input, int *i);
t_token			*handle_pipe(const char *input, int *i);
t_token			*handle_redirect(const char *input, int *i);

// ──────────────────────────────────────────────────────────────
// LEXER: EXPANSION
// ──────────────────────────────────────────────────────────────

char			*expand_string(const char *input, t_shell *shell);
char			*expand_loop(const char *input, t_shell *shell, char *result);
char			*expand_variable(const char *str, int *i, t_shell *shell);
char			*expand_home(const char *input, t_shell *shell);

// ──────────────────────────────────────────────────────────────
// LEXER: ERRORES
// ──────────────────────────────────────────────────────────────

void			print_quote_error(t_quote_error err);
void			print_syntax_error(int err_code);

t_cmd_table		*parse_tokens(t_token *tokens);

// ──────────────────────────────────────────────────────────────
// PARSER: UTILS
// ──────────────────────────────────────────────────────────────

int				count_pipes(t_token *tokens);
int				count_args(t_token *tokens);
void			append_redir(t_token **redirs, t_token *redir);
void			parser_handle_word(t_cmd *cmd, char *content, int *arg_index);
void			handle_redir(t_cmd *cmd, t_token **tmp);
void			fill_command_args(t_cmd *cmd, t_token **tmp, t_token *end);
int				init_cmd_table(t_cmd_table **table, int count);
void			setup_command(t_cmd *cmd);
void			move_to_next_pipe(t_token **current);
void			fill_cmd_table(t_cmd_table *table, t_token *tokens);

// ──────────────────────────────────────────────────────────────
// SIGNALS
// ──────────────────────────────────────────────────────────────

int				signal_flag(int mode, int val);
void			handle_signal_heredoc_interrupt(void);
void			handle_normal_interrupt(void);
void			handle_child_interrupt(void);
void			handle_sigint(void);
void			handle_sigquit(void);
void			handle_signal(int signo);
void			setup_signal_handlers(void);

//BUILTINS
int				is_builtin(char *cmd);
int				execute_builtin(char **argv, t_env **env_list);
int				builtin_echo(char **argv);
int				builtin_pwd(char **argv);
int				builtin_env(char **argv, t_env *env);
int				builtin_exit(char **argv);
int				builtin_cd(char **argv, t_env **env_list);
int				print_export_env(t_env *env);
int				process_single_export(char *arg, t_env **env_list);
int				builtin_export(char **argv, t_env **env_list);
int				builtin_unset(char **argv, t_env **env_list);

//UTILS
int				is_valid_identifier(const char *str);
int				is_numeric(const char *str);
int				str_is_overflowing_long(const char *str);
char			*join_path_cmd(char *path, char *cmd);
char			*search_paths(char **paths, char *cmd);
char			*build_full_path(char *dir, char *cmd);
char			*search_in_paths(char **paths, char *cmd);
char			*find_command_path(char *cmd, t_env *env_list);
size_t			ft_max(size_t a, size_t b);
int				env_len(t_env *env);
void			sort_env_array(t_env **arr, int len);
void			print_export_entry(t_env *var);

//ENV
t_env			*create_env_node(char *entry);
t_env			*env_init(char **envp);
int				split_key_value(char *arg, char **key, char **value);
void			update_value(t_env *env, const char *value);
t_env			*new_env_node(const char *key, const char *value);
void			env_set(t_env **env, const char *key, const char *value);
void			env_unset(t_env **env, const char *key);
int				env_list_size(t_env *env);
char			**env_to_array(t_env *env);
char			*env_get(t_env *env, const char *key);

//LIBERATIONS
void			free_tokens(t_token *tokens);
void			free_cmd_table(t_cmd_table *table);
void			free_env_list(t_env *env);
void			clean_exit(t_env *env_list, t_token *tokens, \
					char *input, int exit_code);
void			print_cmd_table(t_cmd_table *table);
int				validate_cmd_table(t_shell *shell);
char			**get_heredoc_names(t_token *redirs);

#endif
