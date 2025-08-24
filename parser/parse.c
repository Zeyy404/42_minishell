/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/08/24 12:58:49 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int add_argv_with_flags(char ***argv, int **flags, int *argc, char *value, int quote_type)
{
    char **new_argv;
    int  *new_flags;
    int   i;

    new_argv = malloc(sizeof(char *) * ((*argc) + 2));
    if (!new_argv)
        return (0);

    new_flags = malloc(sizeof(int) * ((*argc) + 1));
    if (!new_flags)
        return (free(new_argv), 0);

    i = 0;
    while (i < *argc)
    {
        new_argv[i] = (*argv)[i];
        new_flags[i] = (*flags)[i];
        i++;
    }

    new_argv[i] = ft_strdup(value);
    if (!new_argv[i])
        return (free(new_flags), free(new_argv), 0);

    new_flags[i] = quote_type;
    new_argv[i + 1] = NULL;

    free(*argv);
    free(*flags);

    *argv = new_argv;
    *flags = new_flags;
    (*argc)++;
    return (1);
}

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;
	char	**argv;
	int		argc;
	int		i;
	int		j;
	int *flags = NULL;
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

			int quote_type = 0;
			if ((*tokens)->quotes)
				quote_type = 1;
			else if ((*tokens)->dquotes)
				quote_type = 2;
			// if (!add_str(&argv, &argc, (*tokens)->value))
			// 	return 	(NULL);
			if (!add_argv_with_flags(&argv, &flags, &argc, (*tokens)->value, quote_type))
				return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	node->cmd.argv = argv;
	node->cmd.quotes = flags;
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
