/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:42 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/07/27 11:52:57 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_redirect_out(t_cmd *cmd)
{
	int	fd;

	printf("ececution:::: outfile: %s\n", cmd->outfile);
	if (!cmd->outfile || *cmd->outfile == '\0')
		return ;
	if (cmd->append == 1)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("outfile error\n");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 error\n");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	execute_redirect_in(t_cmd *cmd)
{
	int	fd;


	if (cmd->here_doc == 1)
		execute_herdoc(cmd);
	if (!cmd->infile || *cmd->infile == '\0')
		return ;
	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		perror("error infile\n");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 error\n");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	execute_herdoc(t_cmd *cmd)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
	{
		perror("pipe error\n");
		exit(1);
	}
	while (1)
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
		perror("dup2 error\n");
		close(fd[0]);
		exit(1);
	}
	close(fd[0]);
}
