/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:26:34 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/23 13:59:29 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*ast_new_node(t_node_type nt, t_ast *l, t_ast *r)
{
	t_ast	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->type = nt;
	node->left = l;
	node->right = r;
	ft_memset(&node->cmd, 0, sizeof(t_cmd));
	return (node);
}

t_argv	*new_argv(t_word *words)
{
	t_argv	*node;

	node = malloc(sizeof(t_argv));
	if (!node)
		return (NULL);
	node->words = words;
	node->next = NULL;
	return (node);
}

void	add_argv(t_argv **argv_list, t_word *words)
{
	t_argv	*curr;
	t_argv	*new_node;

	if (!argv_list || !words)
		return ;
	new_node = new_argv(words);
	if (!new_node)
		return ;
	if (!*argv_list)
		*argv_list = new_node;
	else
	{
		curr = *argv_list;
		while (curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

void	transfer_words(t_word **dst, t_word *src)
{
	t_word	*current;

	if (!dst || !src)
		return ;
	if (!*dst)
		*dst = src;
	else
	{
		current = *dst;
		while (current->next)
			current = current->next;
		current->next = src;
	}
}

int	handle_redirection(t_ast *node, t_token **tokens)
{
	if ((*tokens)->type == REDIR_IN)
	{
		*tokens = (*tokens)->next;
		node->cmd.here_doc = 0;
		add_argv(&node->cmd.infile, (*tokens)->words);
		(*tokens)->words = NULL;
	}
	else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
	{
		node->cmd.append = ((*tokens)->type == APPEND);
		*tokens = (*tokens)->next;
		add_argv(&node->cmd.outfile, (*tokens)->words);
		(*tokens)->words = NULL;
	}
	else if ((*tokens)->type == HEREDOC)
	{
		*tokens = (*tokens)->next;
		node->cmd.here_doc = 1;
		transfer_words(&node->cmd.delimiter, (*tokens)->words);
		(*tokens)->words = NULL;
	}
	else
		return (0);
	*tokens = (*tokens)->next;
	return (1);
}
