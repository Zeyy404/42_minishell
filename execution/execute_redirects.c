/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:42 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/08/19 19:33:27 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_redirect_out(t_cmd *cmd)
{
	int	fd;
	int i;
	
	i = 0;
	// printf("ececution:::: outfile: %s\n", cmd->outfile);
	if (!cmd->outfile || **cmd->outfile == '\0')
		return ;
	if (cmd->append == 1)
	{	
		while (cmd->outfile[i])
		{
			fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
			i++;
		}
	}
	else
	{	
		while (cmd->outfile[i])
		{
			fd = open(cmd->outfile[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			i++;
		}
	}
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
	int		i;

	i = 0;
	while (cmd->delimiter[i])
	{
		if (pipe(fd) == -1)
		{
			perror("pipe error\n");
			exit(1);
		}
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, cmd->delimiter[i]) == 0)
			{
				free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		if (cmd->delimiter[i + 1] == NULL)
		{
			if (dup2(fd[0], STDIN_FILENO) == -1)
			{
				perror("dup2 error\n");
				close(fd[0]);
				exit(1);
			}
		}
		close(fd[0]);
		i++;
	}
}
