/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:49:18 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/08/21 00:44:19 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_cmd_path(char *cmd, t_env *env)
{
	char	*path;
	t_env	*tmp;
	char	**split_path;
	int		i;
	char	*joined_path;

	tmp = env;
	path = NULL;
	if (!access(cmd, X_OK))
	{
		return (cmd);
	}
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
		{
			path = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	i = 0;
	while (split_path[i])
	{
		joined_path = concat(split_path[i], '/', cmd);
		if (!access(joined_path, X_OK))
			return (split_free(split_path), joined_path);
		free(joined_path);
		i++;
	}
	split_free(split_path);
	return (NULL);
}

void	execution(t_ast *ast, t_shell *shell, int in_child, int *exit_status)
{
	int in, out;
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		if (ast->cmd.argv && is_builtin(ast->cmd.argv[0], shell))
		{
			if (in_child)
			{
				execute_redirect_in(&ast->cmd);
				execute_redirect_out(&ast->cmd);
				execute_builtins(ast->cmd.argv[0], ast, shell);
			}
			else
			{
				in = dup(STDIN_FILENO);
				out = dup(STDOUT_FILENO);
				execute_redirect_in(&ast->cmd);
				execute_redirect_out(&ast->cmd);
				execute_builtins(ast->cmd.argv[0], ast, shell);
				dup2(in, STDIN_FILENO);
				dup2(out, STDOUT_FILENO);
				close(in);
				close(out);
			}
		}
		else
			execute_one_cmd(ast, shell, exit_status);
	}
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, shell, exit_status);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, shell, exit_status);
	else if (ast->type == NODE_GROUP)
		execute_group(ast, shell, exit_status);
}

void	wait_update_status(pid_t pid, int *exit_status)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		*exit_status = 1;
		return ;
	}
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
}

void	execute_one_cmd(t_ast *ast, t_shell *shell, int *exit_status)
{
	pid_t	pid;
	char	*cmd_path;
	char	**env_arr;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_redirect_in(&ast->cmd);
		execute_redirect_out(&ast->cmd);
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
	else
		wait_update_status(pid, exit_status);
}

void	execute_pipe(t_ast *ast, t_shell *shell, int *exit_status)
{
	int fd[2];
	pid_t left_pid, right_pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	left_pid = fork();
	if (left_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execution(ast->left, shell, 1, exit_status);
		exit(*exit_status);
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execution(ast->right, shell, 1, exit_status);
		exit(*exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	wait_update_status(left_pid, exit_status);
	wait_update_status(right_pid, exit_status);
}

void	execute_and_or(t_ast *ast, t_shell *shell, int *exit_status)
{
	execution(ast->left, shell, 0, exit_status);
	if (ast->type == NODE_AND && *exit_status == 0)
    	execution(ast->right, shell, 0, exit_status);
	else if (ast->type == NODE_OR && *exit_status != 0)
    	execution(ast->right, shell, 0, exit_status);

}

void	execute_group(t_ast *ast, t_shell *shell, int *exit_status)
{
	pid_t pid = fork();

	if (!ast || !ast->left)
	{
		*exit_status = 1;
		return ;
	}
	if (pid == -1)
	{
		perror("fork");
		*exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execution(ast->left, shell, 1, exit_status);
		exit(*exit_status);
	}
	else
	{
		wait_update_status(pid, exit_status);
	}
}
