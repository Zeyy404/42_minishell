/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:26:34 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/13 23:05:41 by zsalih           ###   ########.fr       */
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

// int	add_str(char ***arr, int *count, t_word *words)
// {
// 	char **new_arr;
// 	int	i;
	
// 	new_arr = malloc(sizeof(char *) * ((*count) + 2));
// 	if (!new_arr)
// 		return (0);
// 	i = 0;
// 	while (i < (*count))
// 	{
// 		new_arr[i] = (*arr)[i]; // copying the array of pointers!
// 		i++;
// 	}
// 	while (words)
// 	{
// 		new_arr[i++] = ft_strdup(words->value);
// 		words = words->next;
// 	}
// 	// if (!new_arr[i])
// 	// 	return (free(new_arr), 0);
// 	new_arr[i] = NULL;
// 	free(*arr); // free the previously allocated array of pointers!
// 	*arr = new_arr;
// 	(*count)++;
// 	return (1);
// }
