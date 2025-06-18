#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdlib.h>

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
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
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
}					t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_cmd			cmd;
}					t_ast;

t_ast *ast_new_node(t_node_type nt, t_ast *l, t_ast *r);

// parser

//env
typedef struct s_data
{
	char	**env;
}	t_data;

size_t	env_len(char **env);
char	**init_env(char **envp);
void	ft_putstr(char *str);
void	print_env(char **envp);

#endif