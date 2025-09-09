/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:49:18 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/09 18:31:27 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_ast *ast, t_shell *shell, int in_child)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		if (ast->cmd.argv && is_builtin(ast->cmd.argv[0], shell))
		{
			if (in_child)
				builtin_child(ast, shell);
			else
				builtin_parent(ast, shell);
		}
		else
			execute_one_cmd(ast, shell);
	}
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, shell);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, shell, in_child);
	else if (ast->type == NODE_GROUP)
		execute_group(ast, shell, in_child);
}

void	execute_one_cmd(t_ast *ast, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	g_child_running = 1;
	if (pid == 0)
		exec_child_one(ast, shell);
	else
		wait_update_status(pid, &shell->exit_status);
}

void	execute_pipe(t_ast *ast, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	left_pid = fork();
	g_child_running = 1;
	if (left_pid == 0)
		exec_child_pipe_left(ast, shell, fd);
	right_pid = fork();
	g_child_running = 1;
	if (right_pid == 0)
		exec_child_pipe_right(ast, shell, fd);
	close(fd[0]);
	close(fd[1]);
	wait_update_status(left_pid, &shell->exit_status);
	wait_update_status(right_pid, &shell->exit_status);
}

void	execute_and_or(t_ast *ast, t_shell *shell, int in_child)
{
	execution(ast->left, shell, in_child);
	if (ast->type == NODE_AND && shell->exit_status == 0)
		execution(ast->right, shell, in_child);
	else if (ast->type == NODE_OR && shell->exit_status != 0)
		execution(ast->right, shell, in_child);
}

void	execute_group(t_ast *ast, t_shell *shell, int in_child)
{
	pid_t	pid;

	if (!ast || !ast->left)
	{
		shell->exit_status = 1;
		return ;
	}
	pid = fork();
	g_child_running = 1;
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, sigint);
		signal(SIGQUIT, SIG_DFL);
		if (!in_child && ast->cmd.outfile)
		{
			if (execute_redirect_out(&ast->cmd))
			{
				shell->exit_status = 1;
				exit(1);
			}
		}
		if (ast->cmd.infile)
		{
			if (execute_redirect_in(&ast->cmd, &shell->exit_status))
			{
				shell->exit_status = 1;
				exit(1);
			}
		}
		if (ast->cmd.here_doc)
		{
			if (execute_herdoc(& ast->cmd))
				exit(1);
		}
		execution(ast->left, shell, 1);
		exit(shell->exit_status);
	}
	else
		wait_update_status(pid, &shell->exit_status);
}
