/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:15:39 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/06 15:21:16 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>

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
	char			*outfile;
	int				append;
	int				here_doc;
	char			*delimiter;
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
// parser

// env
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env				*get_env(char **envp);
char				*get_key_value(char *envp, const char *code);
t_env				*new_var(char *key, char *value);
void				add_to_env(t_env **env, t_env *new_var);

// expander
char				*get_env_value(const char *name, t_env *env);
void				get_bounds(char *arg, int *start, int *end);
char				*join_before_after(char *arg, char *value, int start,
						int end);
char				*process_arg(char *arg, t_env *env);
char				*expand_tilde(char *arg, t_env *env);
void				expand_argv(char **argv, t_env *env);
int					expand_word(t_ast *ast, t_env *env);

// builtins

typedef struct s_builtin
{
	char			*name;
	int				(*f)(t_ast *ast, t_env *env);
}					t_builtin;

int					builtin_cd(t_ast *ast, t_env *env);
int					builtin_pwd(t_ast *ast, t_env *env);
int					builtin_echo(t_ast *ast, t_env *env);
int					builtin_export(t_ast *ast, t_env *env);
int					builtin_unset(t_ast *ast, t_env *env);
int					builtin_exit(t_ast *ast, t_env *env);

#endif