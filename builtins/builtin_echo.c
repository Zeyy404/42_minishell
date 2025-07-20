/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:51:24 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/20 11:02:03 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_echo(t_shell *shell)
{
	int	idx;
	int	newline;

	idx = 1;
	newline = 1;
	if (shell->ast->cmd.argv[idx] && ft_strncmp(shell->ast->cmd.argv[idx], "-n", 2) == 0)
	{
		newline = 0;
		idx++;
	}
	while (shell->ast->cmd.argv[idx])
	{
		printf("%s", shell->ast->cmd.argv[idx]);
		if (shell->ast->cmd.argv[idx + 1])
			printf(" ");
		idx++;
	}
	if (newline)
		printf("\n");
	return (0);
}
