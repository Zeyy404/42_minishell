/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:07 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/08/21 00:42:54 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_builtin(char *cmd, t_shell *shell)
{
	t_builtin	builtins[8];
	int			i;

	if (!cmd)
		return (false);
	assign_builtin(builtins, shell);
	i = 0;
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
			return (true);
		i++;
	}
	return (false);
}

void	execute_builtins(char *cmd, t_ast *ast, t_shell *shell)
{
	t_builtin	builtins[8];
	int			i;

	assign_builtin(builtins, shell);
	i = 0;
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
		{
			builtins[i].f(ast, shell);
			break ;
		}
		i++;
	}
}
