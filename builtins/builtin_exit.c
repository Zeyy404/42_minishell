/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:23:35 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 00:05:47 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	if (argv[1] && !is_numeric(argv[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_argv(argv);
		free_shell(shell);
		exit(255);
	}
	else if (argv[1] && argv[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	if (argv[1])
		exit_code = ft_atoi(argv[1]) % 256;
	else
		exit_code = 0;
	free_argv(argv);
	free_shell(shell);
	exit(exit_code);
}
