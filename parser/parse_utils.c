/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:26:34 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/18 13:45:17 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast *ast_new_node(t_node_type nt, t_ast *l, t_ast *r)
{
	t_ast	*new_n;

	new_n = malloc(sizeof(*new_n));
	if (!new_n)
		return (NULL);
	new_n->type = nt;
	new_n->left = l;
	new_n->right = r;
	return (new_n);
}
