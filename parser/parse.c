/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/08/19 19:13:23 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;
	char	**argv;
	int		argc;
	int		i;
	int		j;

	if ((*tokens)->type == LPAREN)
    	return parse_group(tokens);
	node = ast_new_node(NODE_CMD, NULL, NULL);
	argv = NULL;
	argc = 0;
	if (!node)
		return (NULL);
	i = 0;
	j = 0;
	while (*tokens && !is_logic_op((*tokens)->type) && (*tokens)->type != RPAREN)
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
			add_str(&node->cmd.outfile, &i, (*tokens)->value);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
			node->cmd.here_doc = 1;
			add_str(&node->cmd.delimiter, &j, (*tokens)->value);
		}
		else if ((*tokens)->type == WORD)
		{
			if (!add_str(&argv, &argc, (*tokens)->value))
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
	t_ast	*group_node;
	int		i;
	int		j;

	*tokens = (*tokens)->next;
	node = parse_and_or(tokens);
	if (!node || !*tokens || (*tokens)->type != RPAREN)
	{
		free_ast(node);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	group_node = ast_new_node(NODE_GROUP, node, NULL);
	if (!group_node)
	{
		free_ast(node);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (*tokens && is_redir((*tokens)->type))
	{
		if ((*tokens)->type == REDIR_IN)
		{
			*tokens = (*tokens)->next;
			group_node->cmd.infile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
		{
			group_node->cmd.append = ((*tokens)->type == APPEND);
			*tokens = (*tokens)->next;
			add_str(&node->cmd.outfile, &i, (*tokens)->value);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
			add_str(&node->cmd.delimiter, &j, (*tokens)->value);
		}
		*tokens = (*tokens)->next;
	}
	return (group_node);
}

t_ast	*parse(t_token **tokens)
{
	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == LPAREN)
		return (parse_group(tokens));
	return (parse_and_or(tokens));
}

// t_ast *parse(t_token **tokens)
// {
// 	return parse_and_or(tokens);
// }
