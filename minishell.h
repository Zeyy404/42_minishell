/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:15:39 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/17 21:25:34 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

extern int			g_child_running;
// tokenizer
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_OUT,
	REDIR_IN,
	APPEND,
	HEREDOC,
	AND_AND,
	OR_OR,
	AMPERSAND,
	LPAREN,
	RPAREN,
}					t_token_type;

typedef enum e_quote
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE,
}					t_quote;

typedef struct s_word
{
	char			*value;
	t_quote			quote_type;
	struct s_word	*next;
}					t_word;

typedef struct s_token
{
	t_word			*words;
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

t_token				*new_token(t_token_type type, char *value, t_word *words);
void				add_token(t_token **tokens, t_token *new_token);
void				free_tokens(t_token *tokens);
t_word				*new_word(char *value, t_quote quote_type);
void				add_word(t_word **words, t_word *new_word);
void				free_word_list(t_word *words);
t_token				*tokenize(const char *input);
int					token_word(const char *input, size_t *i, t_token **tokens);
int					token_quotes(const char *input, size_t *i, t_word **words);
int					token_operators(const char *input, size_t *i,
						t_token **tokens);
int					check_syntax(t_token *tokens);
int					handle_word(const char *input, size_t *i, t_word **words);
bool				ft_isops(char c);
bool				ft_isspace(char c);
int					is_redir(t_token_type t);
int					is_logic_op(t_token_type t);
int					is_quote(char c);
// tokenizer

// parser
typedef struct s_argv
{
	t_word			*words;
	struct s_argv	*next;
}					t_argv;

typedef struct s_cmd
{
	t_argv			*argv;
	t_word			*infile;
	t_word			*outfile;
	int				append;
	int				here_doc;
	t_word			*delimiter;
}					t_cmd;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_AND,
	NODE_OR,
	NODE_PIPE,
	NODE_GROUP,
}					t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_cmd			cmd;
}					t_ast;

t_ast				*parse_cmd(t_token **tokens);
t_ast				*parse_pipeline(t_token **tokens);
t_ast				*parse_and_or(t_token **tokens);
t_ast				*parse_group(t_token **tokens);
t_ast				*parse(t_token **tokens);
t_ast				*ast_new_node(t_node_type nt, t_ast *l, t_ast *r);
t_argv				*new_argv(t_word *words);
void				add_argv(t_argv **argv_list, t_word *words);
void				transfer_words(t_word **dst, t_word *src);
int					handle_redirection(t_ast *node, t_token **tokens);

// parser

// env
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env			*env;
	t_token			*tokens;
	t_ast			*ast;
	int				exit_status;
}					t_shell;

t_env				*get_env(char **envp);
char				*get_key_value(char *envp, const char *code);
t_env				*new_var(char *key, char *value);
void				add_to_env(t_env **env, t_env *new_var);
void				free_env(t_env *env);

// expander
int					ft_strcmp(const char *s1, const char *s2);
char				*get_env_value(const char *name, t_env *env,
						int exit_status);
void				get_bounds(char *arg, int *start, int *end);
char				*join_before_after(char *arg, char *value, int start,
						int end);
char				*expand_tilde(char *arg, t_env *env, int exit_status);
int					expand_word(t_ast *ast, t_env *env, int exit_status);

// execution
void				split_free(char **split);
char				*concat(char *path, char slash, char *cmd);
char				**env_to_char_array(t_env *env);
char				*get_path(t_env *env);
char				*find_cmd_path(char *cmd, t_env *env);
bool				is_builtin(char *cmd, t_shell *shell);
int					execute_builtins(char *cmd, t_ast *ast, t_shell *shell);
void				wait_update_status(pid_t pid, int *exit_status);
int					builtin_child(t_ast *ast, t_shell *shell);
int					builtin_parent(t_ast *ast, t_shell *shell);
void				exec_child_one(t_ast *ast, t_shell *shell);
void				exec_child_pipe_left(t_ast *ast, t_shell *shell, int fd[2]);
void				exec_child_pipe_right(t_ast *ast, t_shell *shell,
						int fd[2]);
int					open_outfile(char *file, int append, int *fd);
int					execute_redirect_out(t_cmd *cmd);
int					execute_redirect_in(t_cmd *cmd, int *exit_status);
int					heredoc_loop(int *fd, char *delimiter);
int					execute_herdoc(t_cmd *cmd);
void				execution(t_ast *ast, t_shell *shell, int in_child);
void				execute_one_cmd(t_ast *ast, t_shell *shell);
void				execute_pipe(t_ast *ast, t_shell *shell);
void				execute_and_or(t_ast *ast, t_shell *shell, int in_child);
void				execute_group(t_ast *ast, t_shell *shell, int in_child);
char				**flatten_argv(t_argv *argv_list);
void				free_argv(char **argv);

// builtins
typedef struct s_builtin
{
	char			*name;
	int				(*f)(char **, t_shell *);
	t_shell			*shell;
}					t_builtin;

int					builtin_cd(char **argv, t_shell *shell);
int					builtin_pwd(char **argv, t_shell *shell);
int					builtin_echo(char **argv, t_shell *shell);
int					builtin_export(char **argv, t_shell *shell);
int					builtin_unset(char **argv, t_shell *shell);
int					builtin_env(char **argv, t_shell *shell);
int					builtin_exit(char **argv, t_shell *shell);
void				assign_builtin(t_builtin *builtins, t_shell *shell);
void				env_set(t_env **env, char *key, char *value);
char				*env_get(t_env *env, const char *key);
int					is_valid_key(const char *str);

void				free_shell(t_shell *shell);
void				free_ast(t_ast *node);

void				sigint(int sig);
void				set_signals(void);
void				sigint_heredoc(int sig);

void				rl_replace_line(const char *text, int clear_undo);

// print functions (NOT IMPORTANT)
const char			*token_type_str(t_token_type type);
const char			*quote_type_str(t_quote q);
void				print_tokens(t_token *tokens);

#endif