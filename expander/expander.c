/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:50 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/06/24 12:04:51 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_argv(char **argv, t_env *env)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		argv[i] = expand_tilde(argv[i], env);
		argv[i] = process_arg(argv[i], env);
		i++;
	}
}

int	expand_word(t_ast *ast, t_env *env)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && ast->cmd.argv)
	{
		expand_argv(ast->cmd.argv, env);
	}
	if (ast->left)
		expand_word(ast->left, env);
	if (ast->right)
		expand_word(ast->right, env);
	return (1);
}
