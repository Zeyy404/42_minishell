/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:11:16 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/02 13:37:18 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_out(t_cmd *cmd, char *value, int *argc, int quotes, int dquotes)
{
	char **new_out;
	int	*new_out_quotes;
	int i;

	new_out = malloc(sizeof(char *) * ((*argc) + 2));
	new_out_quotes = malloc(sizeof(int) * ((*argc) + 2));
	if (!new_out || !new_out_quotes)
		return 0;
	i = 0;
	while(i < *argc)
	{
		new_out[i] = cmd->outfile[i];
		new_out_quotes[i] = cmd->out_quotes[i];
		i++;
	}
	new_out[i] = ft_strdup(value);
	if (!new_out[i])
		return (0);
	if (quotes)
		new_out_quotes[i] = 1;
	else if (dquotes)
		new_out_quotes[i] = 2;
	else if (!quotes || !dquotes)
		new_out_quotes[i] = 0;
	new_out[i + 1] = NULL;
    new_out_quotes[i + 1] = 0;
	cmd->outfile = new_out;
	cmd->out_quotes = new_out_quotes;
	(*argc)++;
	return (1);
}


int add_a(t_cmd *cmd, char *value, int *argc, int quotes, int dquotes)
{
	char **new_argv;
	int	*neww_quotes;
	int i;

	new_argv = malloc(sizeof(char *) * ((*argc) + 2));
	neww_quotes = malloc(sizeof(int) * ((*argc) + 2));
	if (!new_argv || !neww_quotes)
		return 0;
	i = 0;
	while(i < *argc)
	{
		new_argv[i] = cmd->argv[i];
		neww_quotes[i] = cmd->quotes[i];
		i++;
	}
	new_argv[i] = ft_strdup(value);
	if (!new_argv[i])
		return (0);
	if (quotes)
		neww_quotes[i] = 1;
	else if (dquotes)
		neww_quotes[i] = 2;
	else if (!quotes || !dquotes)
		neww_quotes[i] = 0;
	new_argv[i + 1] = NULL;
    neww_quotes[i + 1] = 0;
	cmd->argv = new_argv;
	cmd->quotes = neww_quotes;
	(*argc)++;
	return (1);
}

t_ast	*parse_cmd(t_token **tokens)
{
	t_ast	*node;
	// char	**argv;
	int		argc;
	int		i;
	int		j;
	if ((*tokens)->type == LPAREN)
    	return parse_group(tokens);
	node = ast_new_node(NODE_CMD, NULL, NULL);
	// argv = NULL;
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
			// add_str(&node->cmd.outfile, &i, (*tokens)->value);
			printf("there is an outfile\n");
			add_out(&node->cmd, (*tokens)->value, &i, (*tokens)->quotes, (*tokens)->dquotes);
		}
		else if ((*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next;
			node->cmd.here_doc = 1;
			add_str(&node->cmd.delimiter, &j, (*tokens)->value);
		}
		else if ((*tokens)->type == WORD)
		{
			if (!add_a(&node->cmd, (*tokens)->value, &argc, (*tokens)->quotes, (*tokens)->dquotes))
				return (NULL);

			// if (!add_str(&argv, &argc, (*tokens)->value))
			// 	return 	(NULL);
		}
		*tokens = (*tokens)->next;
	}
	// node->cmd.argv = argv;
	// int n = 0;
	// while(n < (argc))
	// {
	// 	printf("argv[i]: %s\n", node->cmd.argv[n]);
	// 	printf("quotes[i]: %d\n", node->cmd.quotes[n]);
	// 	n++;
	// }
	// n = 0;
	// while(n < (i))
	// {
	// 	printf("out[i]: %s\n", node->cmd.outfile[n]);
	// 	printf("outquotes[i]: %d\n", node->cmd.out_quotes[n]);
	// 	n++;
	// }
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

// t_ast *parse_group(t_token **tokens)
// {
//     t_ast *inner;
//     t_ast *group_node;
//     int out_count = 0;
//     int heredoc_count = 0;

//     // Skip '('
//     *tokens = (*tokens)->next;

//     // Parse the inside as full AND/OR pipeline
//     inner = parse_and_or(tokens);
//     if (!inner || !*tokens || (*tokens)->type != RPAREN)
//     {
//         free_ast(inner);
//         return NULL;
//     }

//     // Skip ')'
//     *tokens = (*tokens)->next;

//     // Wrap in a NODE_GROUP
//     group_node = ast_new_node(NODE_GROUP, inner, NULL);
//     if (!group_node)
//     {
//         free_ast(inner);
//         return NULL;
//     }

//     // Handle redirections applied to the group as a whole:  (cmd) > file
//     while (*tokens && is_redir((*tokens)->type))
//     {
//         if ((*tokens)->type == REDIR_IN)
//         {
//             *tokens = (*tokens)->next;
//             group_node->cmd.infile = ft_strdup((*tokens)->value);
//         }
//         else if ((*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND)
//         {
//             group_node->cmd.append = ((*tokens)->type == APPEND);
//             *tokens = (*tokens)->next;
//             add_out(&group_node->cmd, (*tokens)->value, &out_count, (*tokens)->quotes, (*tokens)->dquotes);
//         }
//         else if ((*tokens)->type == HEREDOC)
//         {
//             *tokens = (*tokens)->next;
//             add_str(&group_node->cmd.delimiter, &heredoc_count, (*tokens)->value);
//         }
//         *tokens = (*tokens)->next;
//     }

//     return group_node;
// }



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
