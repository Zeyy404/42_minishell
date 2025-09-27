/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:49:18 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/27 12:14:52 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_cmd(t_ast *ast, t_shell *shell, int in_child)
{
	char	**argv;
	pid_t	pid;

	argv = flatten_argv(ast->cmd.argv);
	if (argv && is_builtin(argv[0], shell))
	{
		free_argv(argv);
		if (in_child)
			builtin_child(ast, shell, -1, -1);
		else
			builtin_parent(ast, shell);
	}
	else
	{
		free_argv(argv);
		pid = fork();
		g_signal_mode = 0;
		if (pid == 0)
			exec_child_cmd(ast, shell);
		else
			wait_update_status(pid, &shell->exit_status);
	}
}

static void	execute_pipe(t_ast *ast, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	left_pid = fork();
	g_signal_mode = 0;
	if (left_pid == 0)
		exec_child_pipe_left(ast, shell, fd);
	right_pid = fork();
	g_signal_mode = 0;
	if (right_pid == 0)
		exec_child_pipe_right(ast, shell, fd);
	close(fd[0]);
	close(fd[1]);
	wait_update_status(left_pid, &shell->exit_status);
	wait_update_status(right_pid, &shell->exit_status);
}

static void	execute_and_or(t_ast *ast, t_shell *shell, int in_child)
{
	execution(ast->left, shell, in_child);
	if (ast->type == NODE_AND && shell->exit_status == 0)
		execution(ast->right, shell, in_child);
	else if (ast->type == NODE_OR && shell->exit_status != 0)
		execution(ast->right, shell, in_child);
}

static void	execute_group(t_ast *ast, t_shell *shell)
{
	pid_t	pid;

	if (!ast || !ast->left)
	{
		shell->exit_status = 1;
		return ;
	}
	pid = fork();
	g_signal_mode = 0;
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		group_child(ast, shell);
		free_shell(shell);
		exit(shell->exit_status);
	}
	else
		wait_update_status(pid, &shell->exit_status);
}

void	execution(t_ast *ast, t_shell *shell, int in_child)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
		execute_cmd(ast, shell, in_child);
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, shell);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, shell, in_child);
	else if (ast->type == NODE_GROUP)
		execute_group(ast, shell);
}
