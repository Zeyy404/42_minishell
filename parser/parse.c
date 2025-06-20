/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/20 19:57:43 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;
	char	**argv;
	int		argc;

	node = ast_new_node(NODE_CMD, NULL, NULL);
	argv = NULL;
	argc = 0;
	if (!node)
		return (NULL);
	while (*tokens && !((*tokens)->type == PIPE || (*tokens)->type == AND_AND
		|| (*tokens)->type == OR_OR))
	{
		if ((*tokens)->type == REDIR_IN)
		{
			*tokens = (*tokens)->next;
			node->cmd.infile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
		{
			*tokens = (*tokens)->next;
			node->cmd.append = 1;
			node->cmd.outfile = ft_strdup((*tokens)->value);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
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

t_ast	*parse(t_token **tokens)
{
	if (!*tokens)
		return (NULL);
	return (parse_and_or(tokens));
}
