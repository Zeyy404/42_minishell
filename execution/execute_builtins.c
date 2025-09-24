/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:07 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/24 10:02:43 by zsalih           ###   ########.fr       */
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
	char		**argv;
	int			return_val;

	assign_builtin(builtins, shell);
	i = 0;
	argv = flatten_argv(ast->cmd.argv);
	if (!argv)
		return (1);
	while (builtins[i].name)
	{
		if (!ft_strcmp(cmd, builtins[i].name))
		{
			return_val = builtins[i].f(argv, shell);
			free_argv(argv);
			return (return_val);
		}
		i++;
	}
	free_argv(argv);
	return (1);
}

static int	run_child_func(t_ast *ast, t_shell *shell, int in, int out)
{
	char	**argv;

	argv = flatten_argv(ast->cmd.argv);
	if (ft_strcmp(argv[0], "exit") == 0)
	{
		close_tmp_fds(in, out);
		free_argv(argv);
		shell->exit_status = execute_builtins("exit", ast, shell);
	}
	else
	{
		shell->exit_status = execute_builtins(argv[0], ast, shell);
		if (shell->exit_status == 1)
			close_tmp_fds(in, out);
		free_argv(argv);
		return (0);
	}
	return (1);
}

int	builtin_child(t_ast *ast, t_shell *shell, int in, int out)
{
	if (ast->cmd.infile)
	{
		if (execute_redirect_in(&ast->cmd))
			return (shell->exit_status = 1, 1);
	}
	if (ast->cmd.outfile)
	{
		if (execute_redirect_out(&ast->cmd))
			return (shell->exit_status = 1, 1);
	}
	if (!run_child_func(ast, shell, in, out))
		return (0);
	return (1);
}

void	builtin_parent(t_ast *ast, t_shell *shell)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	if (in == -1)
		return (perror("dup: "));
	out = dup(STDOUT_FILENO);
	if (out == -1)
		return (perror("dup: "));
	if (builtin_child(ast, shell, in, out))
		return ;
	if (dup2(in, STDIN_FILENO) == -1)
		return (perror("dup2: "));
	if (dup2(out, STDOUT_FILENO) == -1)
		return (perror("dup2: "));
	close(in);
	close(out);
}
