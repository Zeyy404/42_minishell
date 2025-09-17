/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:42 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/17 19:34:27 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_outfile(char *file, int append, int *fd)
{
	if (append)
		*fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		printf("%s\n", file);
		perror(file);
		return (1);
	}
	return (0);
}

int	execute_redirect_out(t_cmd *cmd)
{
	int	fd;
	int	err;
	t_word *curr;

	err = 0;
	if (!cmd->outfile)
		return (0);
	curr = cmd->outfile;
	while (curr)
	{
		// printf("file: %s\n", curr->value);
		err = open_outfile(curr->value, cmd->append, &fd);
		if (err)
			return (1);
		if (curr->next)
			close(fd);
		curr = curr->next;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 error\n");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	execute_redirect_in(t_cmd *cmd, int *exit_status)
{
	int	fd;
	t_word *curr;

	if (cmd->here_doc == 1)
	{
		if (execute_herdoc(cmd) == 1)
		{
			*exit_status = 0;
			return (1);
		}
	}
	if (!cmd->infile)
		return (0);
	curr = cmd->infile;
	while (curr)
	{
		fd = open(curr->value, O_RDONLY);
		if (fd == -1)
		{
			perror(curr->value);
			return (1);
		}
		if (curr->next)
			close(fd);
		curr = curr->next;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 error\n");
		close(fd);
		return (1);
	}
	return (close(fd), 0);
}

int	heredoc_loop(int *fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	return (0);
}

int	execute_herdoc(t_cmd *cmd)
{
	int		fd[2];
	t_word *curr;

	curr = cmd->delimiter;
	while (curr)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe error\n");
			return (1);
		}
		signal(SIGINT, sigint_heredoc);
		signal(SIGQUIT, SIG_IGN);
		if (heredoc_loop(fd, curr->value))
			return (close(fd[0]), close(fd[1]), 1);
		close(fd[1]);
		if (curr->next == NULL)
		{
			if (dup2(fd[0], STDIN_FILENO) == -1)
				return (perror("dup2 error\n"), close(fd[0]), 1);
		}
		close(fd[0]);
		curr = curr->next;
	}
	return (0);
}
