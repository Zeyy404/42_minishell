/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 12:23:59 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	add_out(t_cmd *cmd, char *value, int *argc, int quotes, int dquotes)
// {
// 	char **new_out;
// 	int	*new_out_quotes;
// 	int i;

// 	new_out = malloc(sizeof(char *) * ((*argc) + 2));
// 	new_out_quotes = malloc(sizeof(int) * ((*argc) + 2));
// 	if (!new_out || !new_out_quotes)
// 		return 0;
// 	i = 0;
// 	while(i < *argc)
// 	{
// 		new_out[i] = cmd->outfile[i];
// 		new_out_quotes[i] = cmd->out_quotes[i];
// 		i++;
// 	}
// 	new_out[i] = ft_strdup(value);
// 	if (!new_out[i])
// 		return (0);
// 	if (quotes)
// 		new_out_quotes[i] = 1;
// 	else if (dquotes)
// 		new_out_quotes[i] = 2;
// 	else if (!quotes || !dquotes)
// 		new_out_quotes[i] = 0;
// 	new_out[i + 1] = NULL;
//     new_out_quotes[i + 1] = 0;
// 	cmd->outfile = new_out;
// 	cmd->out_quotes = new_out_quotes;
// 	(*argc)++;
// 	return (1);
// }


// int add_a(t_cmd *cmd, char *value, int *argc, int quotes, int dquotes)
// {
// 	char **new_argv;
// 	int	*neww_quotes;
// 	int i;

// 	new_argv = malloc(sizeof(char *) * ((*argc) + 2));
// 	neww_quotes = malloc(sizeof(int) * ((*argc) + 2));
// 	if (!new_argv || !neww_quotes)
// 		return 0;
// 	i = 0;
// 	while(i < *argc)
// 	{
// 		new_argv[i] = cmd->argv[i];
// 		neww_quotes[i] = cmd->quotes[i];
// 		i++;
// 	}
// 	new_argv[i] = ft_strdup(value);
// 	if (!new_argv[i])
// 		return (0);
// 	if (quotes)
// 		neww_quotes[i] = 1;
// 	else if (dquotes)
// 		neww_quotes[i] = 2;
// 	else if (!quotes || !dquotes)
// 		neww_quotes[i] = 0;
// 	new_argv[i + 1] = NULL;
//     neww_quotes[i + 1] = 0;
// 	cmd->argv = new_argv;
// 	cmd->quotes = neww_quotes;
// 	(*argc)++;
// 	return (1);
// }

t_argv  *new_argv(t_word *words)
{
    t_argv *node;

    node = malloc(sizeof(t_argv));
    if (!node)
        return (NULL);
    node->words = words;
    node->next = NULL;
    return (node);
}

void    add_argv(t_argv **argv_list, t_word *words)
{
    t_argv *curr;
    t_argv *new_node;

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

void transfer_words(t_word **dst, t_word *src)
{
    t_word *current;

    if (!dst || !src)
        return;
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

t_ast   *parse_cmd(t_token **tokens)
{
    t_ast   *node;

    if ((*tokens)->type == LPAREN)
        return parse_group(tokens);
    node = ast_new_node(NODE_CMD, NULL, NULL);
    if (!node)
        return (NULL);
    while (*tokens && !is_logic_op((*tokens)->type) && (*tokens)->type != RPAREN)
    {
        if ((*tokens)->type == REDIR_IN)
        {
            *tokens = (*tokens)->next;
            node->cmd.here_doc = 0;
            transfer_words(&node->cmd.infile, (*tokens)->words);
            (*tokens)->words = NULL;
        }
        else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
        {
            node->cmd.append = ((*tokens)->type == APPEND);
            *tokens = (*tokens)->next;
            transfer_words(&node->cmd.outfile, (*tokens)->words);
            (*tokens)->words = NULL;
        }
        else if ((*tokens)->type == HEREDOC)
        {
            *tokens = (*tokens)->next;
            node->cmd.here_doc = 1;
            transfer_words(&node->cmd.delimiter, (*tokens)->words);
            (*tokens)->words = NULL;
        }
        else if ((*tokens)->type == WORD)
        {
            add_argv(&node->cmd.argv, (*tokens)->words);
            (*tokens)->words = NULL;
        }
        *tokens = (*tokens)->next;
    }
    return (node);
}



// t_ast	*parse_cmd(t_token **tokens)
// {
// 	t_ast	*node;
// 	char	**argv;
// 	int		argc;
// 	int		i;
// 	int		j;

// 	if ((*tokens)->type == LPAREN)
//     	return parse_group(tokens);
// 	node = ast_new_node(NODE_CMD, NULL, NULL);
// 	argv = NULL;
// 	argc = 0;
// 	if (!node)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (*tokens && !is_logic_op((*tokens)->type) && (*tokens)->type != RPAREN)
// 	{
// 		if ((*tokens)->type == REDIR_IN)
// 		{
// 			*tokens = (*tokens)->next;
// 			node->cmd.here_doc = 0;
// 			node->cmd.infile = ft_strdup((*tokens)->words->value);
// 		}
// 		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
// 		{
// 			if ((*tokens)->type == REDIR_OUT)
// 				node->cmd.append = 0;
// 			else
// 				node->cmd.append = 1;
// 			*tokens = (*tokens)->next;
// 			add_str(&node->cmd.outfile, &i, (*tokens)->words);
// 			// printf("there is an outfile\n");
// 			// add_out(&node->cmd, (*tokens)->value, &i, (*tokens)->quotes, (*tokens)->dquotes);
// 		}
// 		else if ((*tokens)->type == HEREDOC)
// 		{
// 			*tokens = (*tokens)->next;
// 			node->cmd.here_doc = 1;
// 			add_str(&node->cmd.delimiter, &j, (*tokens)->words);
// 		}
// 		else if ((*tokens)->type == WORD)
// 		{
// 			// if (!add_a(&node->cmd, (*tokens)->value, &argc, (*tokens)->quotes, (*tokens)->dquotes))
// 				// return (NULL);
// 			// t_word *w = (*tokens)->words;
// 			// while (w)
// 			// {
// 			// 	printf("debug: value = [%s]\n", (*tokens)->words->value);
// 			// 	w = w->next;
// 			// }
// 			if (!add_str(&argv, &argc, (*tokens)->words))
// 				return 	(NULL);
// 		}
// 		*tokens = (*tokens)->next;
// 	}
// 	// node->cmd.argv = argv;
// 	// int n = 0;
// 	// while(n < (argc))
// 	// {
// 	// 	printf("argv[i]: %s\n", node->cmd.argv[n]);
// 	// 	printf("quotes[i]: %d\n", node->cmd.quotes[n]);
// 	// 	n++;
// 	// }
// 	// n = 0;
// 	// while(n < (i))
// 	// {
// 	// 	printf("out[i]: %s\n", node->cmd.outfile[n]);
// 	// 	printf("outquotes[i]: %d\n", node->cmd.out_quotes[n]);
// 	// 	n++;
// 	// }
// 	return (node);
// }

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
	{
		free_ast(inner);
		return (NULL);
	}
	while (*tokens && is_redir((*tokens)->type))
	{
		if ((*tokens)->type == REDIR_IN)
		{
			*tokens = (*tokens)->next;
			transfer_words(&group_node->cmd.infile, (*tokens)->words);
            (*tokens)->words = NULL;
		}
		else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
		{
			group_node->cmd.append = ((*tokens)->type == APPEND);
			*tokens = (*tokens)->next;
			transfer_words(&group_node->cmd.outfile, (*tokens)->words);
            (*tokens)->words = NULL;
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
			transfer_words(&group_node->cmd.delimiter, (*tokens)->words);
            (*tokens)->words = NULL;
			group_node->cmd.here_doc = 1;
		}
		*tokens = (*tokens)->next;
	}
	return (group_node);
}

// t_ast	*parse(t_token **tokens)
// {
// 	if (!*tokens)
// 		return (NULL);
// 	if ((*tokens)->type == LPAREN)
// 		return (parse_group(tokens));
// 	return (parse_and_or(tokens));
// }

t_ast *parse(t_token **tokens)
{
    if (!*tokens)
        return (NULL);
    return parse_and_or(tokens);
}


// t_ast *parse(t_token **tokens)
// {
// 	return parse_and_or(tokens);
// }
