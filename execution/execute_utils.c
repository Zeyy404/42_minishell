/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:51:57 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/03 15:55:05 by yalkhidi         ###   ########.fr       */
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

void	exec_child_one(t_ast *ast, t_shell *shell, int *exit_status)
{
	char	*cmd_path;
	char	**env_arr;

	signal(SIGINT, sigint);
	signal(SIGQUIT, SIG_DFL);
	if (execute_redirect_in(&ast->cmd, exit_status))
		exit(1);
	if (execute_redirect_out(&ast->cmd))
		exit(1);
	if (!ast->cmd.argv || !ast->cmd.argv[0])
		exit(0);
	cmd_path = find_cmd_path(ast->cmd.argv[0], shell->env);
	env_arr = env_to_char_array(shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd(ast->cmd.argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		split_free(env_arr);
		exit(127);
	}
	execve(cmd_path, ast->cmd.argv, env_arr);
	perror("execve");
	split_free(env_arr);
	free(cmd_path);
	exit(1);
}

void	exec_child_pipe_left(t_ast *ast, t_shell *shell, int *exit_status,
	int fd[2])
{
	signal(SIGINT, sigint);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execution(ast->left, shell, 1, exit_status);
	exit(*exit_status);
}

void	exec_child_pipe_right(t_ast *ast, t_shell *shell, int *exit_status,
	int fd[2])
{
	signal(SIGINT, sigint);
	signal(SIGQUIT, SIG_DFL);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	execution(ast->right, shell, 1, exit_status);
	exit(*exit_status);
}
