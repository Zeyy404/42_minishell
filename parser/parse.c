/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/13 08:12:52 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;
	char	**argv;
	int		argc;

	if ((*tokens)->type == LPAREN)
    	return parse_group(tokens);
	node = ast_new_node(NODE_CMD, NULL, NULL);
	argv = NULL;
	argc = 0;
	if (!node)
		return (NULL);
	while (*tokens && !is_logic_op((*tokens)->type))
	{
		if ((*tokens)->type == REDIR_IN)
		{
			*tokens = (*tokens)->next;
			node->cmd.here_doc = 0;
			node->cmd.infile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
		{
			if ((*tokens)->type == REDIR_OUT)
				node->cmd.append = 0;
			else
				node->cmd.append = 1;	
			*tokens = (*tokens)->next;
			node->cmd.outfile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
			node->cmd.here_doc = 1;
			node->cmd.delimiter = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == WORD)
		{
			if (!add_argv(&argv, &argc, (*tokens)->value))
				return 	(NULL);
		}
		*tokens = (*tokens)->next;
	}
	node->cmd.argv = argv;
	return (node);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	nt;

	left = parse_cmd(tokens);
	while (*tokens && (*tokens)->type == PIPE)
	{
		nt = NODE_PIPE;
		*tokens = (*tokens)->next;
		right = parse_cmd(tokens);
		left = ast_new_node(nt, left, right);
	}
	return (left);
}

t_ast	*parse_and_or(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	nt;

	left = parse_pipeline(tokens);
	while (*tokens && ((*tokens)->type == AND_AND || (*tokens)->type == OR_OR))
	{
		if ((*tokens)->type == AND_AND)
			nt = NODE_AND;
		else
			nt = NODE_OR;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		left = ast_new_node(nt, left, right);
	}
	return (left);
}

t_ast	*parse_group(t_token **tokens)
{
	t_ast	*node;

	*tokens = (*tokens)->next;
	node = parse_and_or(tokens);
	*tokens = (*tokens)->next; // Skip RPAREN
	while (*tokens && is_redir((*tokens)->type))
	{
		if ((*tokens)->type == REDIR_IN)
		{
			node->cmd.infile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
		{
			node->cmd.append = 1;
			node->cmd.outfile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			node->cmd.delimiter = ft_strdup((*tokens)->value);
		}
		*tokens = (*tokens)->next;
	}
	node->type = NODE_GROUP;
	return (node);
}

t_ast	*parse(t_token **tokens)
{
	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == LPAREN)
		return (parse_group(tokens));
	return (parse_and_or(tokens));
}
