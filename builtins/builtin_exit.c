/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 18:23:35 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/12 18:45:24 by zsalih           ###   ########.fr       */
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

int	builtin_exit(t_ast *ast, t_env **env)
{
	int	exit_code;

	(void)env;
	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", 2);
	if (ast->cmd.argv[1] && !is_numeric(ast->cmd.argv[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(ast->cmd.argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	else if (ast->cmd.argv[1] && ast->cmd.argv[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	if (ast->cmd.argv[1])
		exit_code = ft_atoi(ast->cmd.argv[1]) % 256;
	else
		exit_code = 0;
	exit(exit_code);
}
