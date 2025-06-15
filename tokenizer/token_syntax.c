#include "../minishell.h"

static int	is_logic_op(t_token_type t)
{
	return (t == PIPE || t == AND_AND || t == OR_OR || t == AMPERSAND);
}

static int	is_redir(t_token_type t)
{
	return (t == REDIR_IN || t == REDIR_OUT || t == APPEND || t == HEREDOC);
}

static int	is_operator(t_token_type t)
{
	return (is_logic_op(t) || is_redir(t));
}

int	check_syntax(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (!prev && is_logic_op(tokens->type))
		{
			printf("syntax error near unexpected token `%s'\n", tokens->value);
			return (0);
		}
		else if (prev && is_operator(prev->type) && is_operator(tokens->type))
		{
			printf("syntax error near unexpected token `%s'\n", tokens->value);
			return (0);
		}
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && (is_logic_op(prev->type) || is_redir(prev->type)))
	{
		printf("syntax error near unexpected token `newline'\n");
		return (0);
	}
	return (1);
}
