
#include "../minishell.h"
#include <fcntl.h>

bool	is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", ft_strlen(cmd)) || !ft_strncmp(cmd, "cd",
			ft_strlen(cmd)) || !ft_strncmp(cmd, "pwd", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "export", ft_strlen(cmd)) || !ft_strncmp(cmd,
			"unset", ft_strlen(cmd)) || !ft_strncmp(cmd, "env", ft_strlen(cmd))
		|| !ft_strncmp(cmd, "exit", ft_strlen(cmd)));
}

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
		execute_one_cmd(ast, env);
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, env);
}

void	execute_one_cmd(t_ast *ast, t_env *env)
{
	pid_t pid;
	char *cmd_path;
	char **env_arr;

	pid = fork();
	if (pid == 0)
	{
		execute_redirects(&ast->cmd);
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
		wait(NULL);
	}
}

void	execute_pipe(t_ast *ast, t_env *env)
{
	int	fd[2];
	pid_t	pid1;
	pid_t	pid2;

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
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	execute_redirects(t_cmd *cmd)
{
	int fd;

	printf("append int: %d\n", cmd->append);
	if (cmd->outfile)
	{
		if (cmd->append == 1)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			printf("error\n");
			exit(1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			printf("error\n");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}