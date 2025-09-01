/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:15:39 by zsalih            #+#    #+#             */
/*   Updated: 2025/08/24 11:51:05 by yalkhidi         ###   ########.fr       */
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
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

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

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quotes;
	int				dquotes;
	struct s_token	*next;
}					t_token;

t_token				*new_token(t_token_type type, char *value);
void				add_token(t_token **tokens, t_token *new_token);
void				free_tokens(t_token *tokens);
t_token				*tokenize(const char *input);
int					token_word(const char *input, size_t *i, t_token **tokens);
int					token_quotes(const char *input, size_t *i,
						t_token **tokens);
int					token_operators(const char *input, size_t *i,
						t_token **tokens);
int					check_syntax(t_token *tokens);
bool				ft_isops(char c);
bool				ft_isspace(char c);
int					is_redir(t_token_type t);
int					is_logic_op(t_token_type t);
// tokenizer

// parser
typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			**outfile;
	int				append;
	int				here_doc;
	char			**delimiter;
	int				*quotes;
	int				*out_quotes;
	int				argc;
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

t_ast				*ast_new_node(t_node_type nt, t_ast *l, t_ast *r);
int					add_argv(char ***argv, int *argc, char *value);
t_ast				*ast_new_node(t_node_type nt, t_ast *l, t_ast *r);
int					add_str(char ***arr, int *count, char *value);
t_ast				*parse_cmd(t_token **tokens);
t_ast				*parse_pipeline(t_token **tokens);
t_ast				*parse_and_or(t_token **tokens);
t_ast				*parse_group(t_token **tokens);
t_ast				*parse(t_token **tokens);
// parser

// env
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef	struct s_shell
{
	t_env	*env;
	t_token	*tokens;
	t_ast	*ast;
}	t_shell;

t_env				*get_env(char **envp);
char				*get_key_value(char *envp, const char *code);
t_env				*new_var(char *key, char *value);
void				add_to_env(t_env **env, t_env *new_var);
void				free_env(t_env *env);

// expander
int					ft_strcmp(char *s1, char *s2);
char				*get_env_value(const char *name, t_env *env, int exit_status);
void				get_bounds(char *arg, int *start, int *end);
char				*join_before_after(char *arg, char *value, int start,
						int end);
char				*process_arg(char *arg, t_env *env, int exit_status);
char				*expand_tilde(char *arg, t_env *env, int exit_status);
void				expand_argv(char **argv, t_env *env, int *flag, int exit_status);
void	expand_files(t_ast *ast, t_env *env, int *flag, int exit_status);
int					expand_word(t_ast *ast, t_env *env, int exit_status);

// execution
void				split_free(char **split);
char				*concat(char *path, char slash, char *cmd);
char				**env_to_char_array(t_env *env);
char				*find_cmd_path(char *cmd, t_env *env);
void				execution(t_ast *ast, t_shell *shell, int in_child, int *exit_status);
void				execute_one_cmd(t_ast *ast, t_shell *shell, int *exit_status);
void				execute_pipe(t_ast *ast, t_shell *shell, int *exit_status);
void				execute_redirect_in(t_cmd *cmd);
void				execute_redirect_out(t_cmd *cmd);
void				execute_herdoc(t_cmd *cmd);
bool	is_builtin(char *cmd, t_shell *shell);
void	execute_builtins(char *cmd, t_ast *ast, t_shell *shell);
void				execute_and_or(t_ast *ast, t_shell *shell, int *exit_status);
void				execute_group(t_ast *ast, t_shell *shell, int *exit_status);

// builtins
typedef struct s_builtin
{
	char			*name;
	int				(*f)(t_ast *, t_shell *);
	t_shell			*shell;
}					t_builtin;

int					builtin_cd(t_ast *ast, t_shell *shell);
int					builtin_pwd(t_ast *ast, t_shell *shell);
int					builtin_echo(t_ast *ast, t_shell *shell);
int					builtin_export(t_ast *ast, t_shell *shell);
int					builtin_unset(t_ast *ast, t_shell *shell);
int					builtin_env(t_ast *ast, t_shell *shell);
int					builtin_exit(t_ast *ast, t_shell *shell);
void				assign_builtin(t_builtin *builtins, t_shell *shell);
void				env_set(t_env **env, char *key, char *value);
char				*env_get(t_env *env, const char *key);
int					is_valid_key(const char *str);

void	free_shell(t_shell	*shell);
void	free_ast(t_ast *ast);
#endif