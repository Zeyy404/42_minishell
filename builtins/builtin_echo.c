/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:51:24 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 00:04:06 by zsalih           ###   ########.fr       */
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

int	builtin_echo(char **argv, t_shell *shell)
{
	int	idx;
	int	newline;

	(void)shell;
	if (!argv)
		return (1);
	idx = 1;
	newline = 1;
	while (argv[idx] && is_all_n(argv[idx]))
	{
		newline = 0;
		idx++;
	}
	while (argv[idx])
	{
		printf("%s", argv[idx]);
		if (argv[idx + 1])
			printf(" ");
		idx++;
	}
	if (newline)
		printf("\n");
	return (0);
}
