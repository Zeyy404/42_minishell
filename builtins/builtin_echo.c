/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:51:24 by zsalih            #+#    #+#             */
/*   Updated: 2025/08/21 00:37:42 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_all_n(char *s)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return (0);
	i++;
	if (s[i] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	builtin_echo(t_ast *ast, t_shell *shell)
{
	int	idx;
	int	newline;

	(void)shell;
	idx = 1;
	newline = 1;
	while (ast->cmd.argv[idx] && is_all_n(ast->cmd.argv[idx]))
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
