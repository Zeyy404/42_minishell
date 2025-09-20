/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:51:57 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/20 10:39:26 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_update_status(pid_t pid, int *exit_status)
{
	int	status;
	int	sig;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		*exit_status = 1;
		return ;
	}
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			printf("Quit: %d\n", sig);
		else if (sig == SIGINT)
			printf("\n");
		*exit_status = 128 + sig;
	}
}

void	exec_child_pipe_left(t_ast *ast, t_shell *shell, int fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execution(ast->left, shell, 1);
	free_shell(shell);
	exit(shell->exit_status);
}

void	exec_child_pipe_right(t_ast *ast, t_shell *shell, int fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	execution(ast->right, shell, 1);
	free_shell(shell);
	exit(shell->exit_status);
}

void	group_child(t_ast *ast, t_shell *shell, int in_child)
{
	signal(SIGINT, SIG_DFL);
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
		if (execute_redirect_in(&ast->cmd))
		{
			shell->exit_status = 1;
			exit(1);
		}
	}
	if (ast->cmd.here_doc)
	{
		if (execute_herdoc(&ast->cmd))
			exit(1);
	}
	execution(ast->left, shell, 1);
}
