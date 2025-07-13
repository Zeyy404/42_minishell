/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:51:24 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/13 15:16:15 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(t_ast *ast, t_env **env)
{
	int	idx;
	int	newline;

	idx = 1;
	newline = 1;
	(void)env;
	if (ast->cmd.argv[idx] && ft_strncmp(ast->cmd.argv[idx], "-n", 2) == 0)
	{
		newline = 0;
		idx++;
	}
	while (ast->cmd.argv[idx])
	{
		printf("%s", ast->cmd.argv[idx]);
		if (ast->cmd.argv[idx + 1])
			printf(" ");
		idx++;
	}
	if (newline)
		printf("\n");
	return (0);
}
