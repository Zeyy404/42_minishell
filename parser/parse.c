/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/17 21:17:30 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;

	if ((*tokens)->type == LPAREN)
		return (parse_group(tokens));
	node = ast_new_node(NODE_CMD, NULL, NULL);
	if (!node)
		return (NULL);
	while (*tokens && !is_logic_op((*tokens)->type)
		&& (*tokens)->type != RPAREN)
	{
		if (is_redir((*tokens)->type))
		{
			if (!handle_redirection(node, tokens))
				return (NULL);
		}
		else if ((*tokens)->type == WORD)
		{
			add_argv(&node->cmd.argv, (*tokens)->words);
			(*tokens)->words = NULL;
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
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
	t_ast	*inner;
	t_ast	*group_node;

	*tokens = (*tokens)->next;
	inner = parse_and_or(tokens);
	if (!inner || !*tokens || (*tokens)->type != RPAREN)
	{
		free_ast(inner);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	group_node = ast_new_node(NODE_GROUP, inner, NULL);
	if (!group_node)
		return (free_ast(inner), NULL);
	while (*tokens && is_redir((*tokens)->type))
	{
		if (!handle_redirection(group_node, tokens))
			return (free_ast(group_node), NULL);
	}
	return (group_node);
}

t_ast	*parse(t_token **tokens)
{
	if (!*tokens)
		return (NULL);
	return (parse_and_or(tokens));
}
