/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:26:34 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/27 15:24:47 by yalkhidi         ###   ########.fr       */
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

void	add_redirect(t_redirects **redirect_list, int type, t_word *words)
{
	t_redirects	*new_node;
	t_redirects	*curr;

	new_node = malloc(sizeof(t_redirects));
	if (!new_node)
		return ;
	new_node->type = type;
	new_node->files = new_argv(words);
	new_node->next = NULL;
	if (!*redirect_list)
		*redirect_list = new_node;
	else
	{
		curr = *redirect_list;
		while (curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

int	handle_redirection(t_ast *node, t_token **tokens)
{
	int	type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != WORD)
		return (0);
	if (type == REDIR_IN)
		add_redirect(&node->cmd.redirections, REDIR_IN, (*tokens)->words);
	else if (type == REDIR_OUT)
		add_redirect(&node->cmd.redirections, REDIR_OUT, (*tokens)->words);
	else if (type == APPEND)
	{
		node->cmd.append = 1;
		add_redirect(&node->cmd.redirections, APPEND, (*tokens)->words);
	}
	else if (type == HEREDOC)
	{
		node->cmd.here_doc = 1;
		add_redirect(&node->cmd.redirections, HEREDOC, (*tokens)->words);
	}
	else
		return (0);
	(*tokens)->words = NULL;
	*tokens = (*tokens)->next;
	return (1);
}
