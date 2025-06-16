#include "../minishell.h"

t_ast *parse_cmd(t_token **tokens)
{
    // highest-level precedence 
}

t_ast *parse_pipeline(t_token **tokens)
{
    // mid-level precedence
}

t_ast *parse_and_or(t_token **tokens)
{
    t_ast *left;
    t_ast *right;
    t_node_type nt;

    left = parse_pipeline(tokens);
    if (!left)
        return (NULL);
    while (*tokens && ((*tokens)->type == AND_AND || (*tokens)->type == OR_OR))
    {
        if ((*tokens)->type == AND_AND)
            nt = AND_AND;
        else
            nt = OR_OR;
        *tokens = (*tokens)->next;
        right = parse_pipeline(tokens);
        // error mangement; if right is NULL free left
        // create a new AST node and set it as the next left node and assign the above left and right as its childern!
    }
    return (left);
}

t_ast   *parse(t_token **tokens)
{
    return (parse_and_or(tokens));
}

