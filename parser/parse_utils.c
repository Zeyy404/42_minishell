/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:26:34 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/19 16:55:47 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast *ast_new_node(t_node_type nt, t_ast *l, t_ast *r)
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

int	add_argv(char ***argv, int *argc, char *value)
{
	char **new_argv;
	int	i;
	
	new_argv = malloc(sizeof(char *) * ((*argc) + 2));
	if (!new_argv)
		return (NULL);
	i = 0;
	while (i < (*argc))
	{
		new_argv[i] = (*argv)[i]; // copying the array of pointers!
		i++;
	}
	new_argv[i] = ft_strdup(value);
	if (new_argv[i])
		return (free(new_argv), 0);
	new_argv[i + 1] = '\0';
	free(*argv); // free the previously allocated array of pointers!
	*argv = new_argv;
	(*argc)++;
	return (1);
}
