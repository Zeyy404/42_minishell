#include "../minishell.h"

void	execute_redirect_out(t_cmd *cmd)
{
	int fd;

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

void	execute_redirect_in(t_cmd *cmd)
{
	int fd;

	if (cmd->infile && cmd->here_doc == 0)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
		{
			printf("error infile\n");
			exit(1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			printf("error\n");
			exit(1);
		}
		close(fd);
	}
	else if (cmd->here_doc == 1)
		execute_herdoc(cmd);
}

void	execute_herdoc(t_cmd *cmd)
{
	int fd[2];
	char *line;

	if (pipe(fd) == -1)
	{
		printf("error\n");
		exit(1);
	}
	while(1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, cmd->delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		printf("error\n");
		close(fd[0]);
		exit(1);
	}
	close(fd[0]);
}
