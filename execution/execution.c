#include "../minishell.h"

char *find_cmd_path(char *cmd, t_env *env)
{
	char *path;
	t_env *tmp;
	char **split_path;
	int i;
	char *joined_path;

	tmp = env;
	path = NULL;
	while(tmp)
	{
		if (!ft_strncmp(tmp->key, "PATH", 5))
		{
			path = tmp->value;
			break;
		}
		tmp = tmp->next;
	}
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	i = 0;
	while(split_path[i])
	{
		joined_path = concat(split_path[i], '/', cmd);
		if (!access(joined_path, X_OK))
			return(split_free(split_path), joined_path);
		free(joined_path);
		i++;
	}
	split_free(split_path);
	return (NULL);
	
}

void	execution(t_ast *ast, t_env *env)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		if (ast->cmd.argv && is_builtin(ast->cmd.argv[0]))
			execute_builtins(ast, env, ast->cmd.argv[0]);
		else
			execute_one_cmd(ast, env);
	}
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, env);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, env);
	else if (ast->type == NODE_GROUP)
		execute_group(ast->left, env);
}

void	execute_one_cmd(t_ast *ast, t_env *env)
{
	pid_t pid;
	char *cmd_path;
	char **env_arr;
	int status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		execute_redirect_in(&ast->cmd);
		execute_redirect_out(&ast->cmd);
		cmd_path = find_cmd_path(ast->cmd.argv[0], env);
		env_arr = env_to_char_array(env);
		if (!cmd_path || !env_arr)
		{
			ft_putstr_fd(ast->cmd.argv[0], 2);
			ft_putendl_fd(" :command not found", 2);
			split_free(env_arr);
			exit(127);
		}
		execve(cmd_path, ast->cmd.argv, env_arr);
		perror("exec failed\n");
		split_free(env_arr);
		exit(1);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
}

void	execute_pipe(t_ast *ast, t_env *env)
{
	int	fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int status;
	int status2;

	status = 0;
	status2 = 0;
	if (!ast)
		return ;
	if (pipe(fd) == -1)
		ft_putendl_fd("error\n", 2);
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execution(ast->left, env);
		exit(1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
		execution(ast->right, env);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status2))
			g_exit_status = WEXITSTATUS(status2);
		else if (WIFSIGNALED(status2))
			g_exit_status = 128 + WTERMSIG(status2);
}

void	execute_and_or(t_ast *ast, t_env *env)
{
	if (!ast)
		return ;
	if (ast->type == NODE_AND)
	{
		execution(ast->left, env);
		if (g_exit_status == 0)
			execution(ast->right, env);
	}
	else if (ast->type == NODE_OR)
	{
		execution(ast->left, env);
		if (g_exit_status != 0)
			execution(ast->right, env);
	}
}

void	execute_group(t_ast *ast, t_env *env)
{
	int	status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (ast->cmd.delimiter || ast->cmd.infile || ast->cmd.outfile)
		{
			execute_redirect_in(&ast->cmd);
			execute_redirect_out(&ast->cmd);
		}
		execution(ast->left, env);
		exit(g_exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
}