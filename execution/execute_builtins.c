/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:07 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/14 00:09:51 by zsalih           ###   ########.fr       */
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

int	execute_builtins(char *cmd, t_ast *ast, t_shell *shell)
{
	t_builtin	builtins[8];
	int			i;
	char **argv;

	assign_builtin(builtins, shell);
	i = 0;
	argv = flatten_argv(ast->cmd.argv);
	if (!argv)
		return (1);
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
		{
			return(builtins[i].f(argv, shell));
			// break ;
		}
		i++;
	}
	return (1);
}

int	builtin_child(t_ast *ast, t_shell *shell)
{
	char **argv;

	if (execute_redirect_in(&ast->cmd, &shell->exit_status))
	{
		shell->exit_status = 1;
		return (1);
	}
	if (execute_redirect_out(&ast->cmd))
	{
		shell->exit_status = 1;
		return (1);
	}
	argv = flatten_argv(ast->cmd.argv);
	shell->exit_status = execute_builtins(argv[0], ast, shell);
	return (0);
}

int	builtin_parent(t_ast *ast, t_shell *shell)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (builtin_child(ast, shell))
		return (1);
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	return (0);
}
