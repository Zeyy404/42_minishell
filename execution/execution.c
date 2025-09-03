/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:49:18 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/03 16:06:00 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_ast *ast, t_shell *shell, int in_child, int *exit_status)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		if (ast->cmd.argv && is_builtin(ast->cmd.argv[0], shell))
		{
			if (in_child)
				builtin_child(ast, shell, exit_status);
			else
				builtin_parent(ast, shell, exit_status);
		}
		else
			execute_one_cmd(ast, shell, exit_status);
	}
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, shell, exit_status);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, shell, exit_status, in_child);
	else if (ast->type == NODE_GROUP)
		execute_group(ast, shell, exit_status);
}

void	execute_one_cmd(t_ast *ast, t_shell *shell, int *exit_status)
{
	pid_t	pid;

	pid = fork();
	g_child_running = 1;
	if (pid == 0)
		exec_child_one(ast, shell, exit_status);
	else
		wait_update_status(pid, exit_status);
}

void	execute_pipe(t_ast *ast, t_shell *shell, int *exit_status)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	left_pid = fork();
	g_child_running = 1;
	if (left_pid == 0)
		exec_child_pipe_left(ast, shell, exit_status, fd);
	right_pid = fork();
	g_child_running = 1;
	if (right_pid == 0)
		exec_child_pipe_right(ast, shell, exit_status, fd);
	close(fd[0]);
	close(fd[1]);
	wait_update_status(left_pid, exit_status);
	wait_update_status(right_pid, exit_status);
}

void	execute_and_or(t_ast *ast, t_shell *shell, int *exit_status,
	int in_child)
{
	execution(ast->left, shell, in_child, exit_status);
	if (ast->type == NODE_AND && *exit_status == 0)
		execution(ast->right, shell, in_child, exit_status);
	else if (ast->type == NODE_OR && *exit_status != 0)
		execution(ast->right, shell, in_child, exit_status);
}

void	execute_group(t_ast *ast, t_shell *shell, int *exit_status)
{
	pid_t	pid;

	if (!ast || !ast->left)
	{
		*exit_status = 1;
		return ;
	}
	pid = fork();
	g_child_running = 1;
	if (pid == -1)
	{
		perror("fork");
		*exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, sigint);
		signal(SIGQUIT, SIG_DFL);
		execution(ast->left, shell, 1, exit_status);
		exit(*exit_status);
	}
	else
		wait_update_status(pid, exit_status);
}
