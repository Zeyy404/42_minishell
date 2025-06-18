#include "../minishell.h"

t_ast *parse_cmd(t_token **tokens)
{
    t_ast *n;
    
    n = malloc(sizeof(*n));
    if (!n)
        return (NULL);
    while (*tokens)
    {
        if ((*tokens)->type == REDIR_IN)
            
        *tokens = (*tokens)->next;
    }
}

t_ast *parse_pipeline(t_token **tokens)
{
    t_ast *left;
    t_ast *right;
    t_node_type nt;

    left = parse_cmd(tokens);
    while (*tokens && (*tokens)->type == PIPE)
    {
        nt = PIPE;
        *tokens = (*tokens)->next;
        right = parse_cmd(tokens);
        left = ast_new_node(nt, left, right);
    }
    return (left);
}

t_ast *parse_and_or(t_token **tokens)
{
    t_ast *left;
    t_ast *right;
    t_node_type nt;

    left = parse_pipeline(tokens);
    while (*tokens && ((*tokens)->type == AND_AND || (*tokens)->type == OR_OR))
    {
        if ((*tokens)->type == AND_AND)
            nt = AND_AND;
        else
            nt = OR_OR;
        *tokens = (*tokens)->next;
        right = parse_pipeline(tokens);
        left = ast_new_node(nt, left, right);
    }
    return (left);
}

t_ast   *parse(t_token **tokens)
{
    return (parse_and_or(tokens));
}

