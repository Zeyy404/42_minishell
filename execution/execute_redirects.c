/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:42 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/23 19:19:05 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_outfile(char *file, int append, int *fd)
{
	if (file[0] == '\0')
		return (1);
	if (append)
		*fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		perror(file);
		return (1);
	}
	return (0);
}

int	execute_redirect_out(t_cmd *cmd)
{
	int		fd;
	int		err;
	int		i;
	char	**outfile;

	outfile = flatten_argv(cmd->outfile);
	if (!outfile)
		return (0);
	err = 0;
	i = 0;
	while (outfile[i])
	{
		err = open_outfile(outfile[i], cmd->append, &fd);
		if (err)
			return (free_argv(outfile), 1);
		if (outfile[i + 1])
			close(fd);
		i++;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (free_argv(outfile), perror("dup2: "), close(fd), 1);
	close(fd);
	return (free_argv(outfile), 0);
}

int	execute_redirect_in(t_cmd *cmd)
{
	int		fd;
	char	**infile;
	int		i;

	infile = flatten_argv(cmd->infile);
	if (!infile)
		return (0);
	i = 0;
	if (cmd->here_doc == 1)
	{
		if (execute_herdoc(cmd) == 1)
			return (free_argv(infile), 1);
	}
	while (infile[i])
	{
		fd = open(infile[i], O_RDONLY);
		if (fd == -1)
			return (free_argv(infile), perror(infile[i]), 1);
		if (infile[i + 1])
			close(fd);
		i++;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (free_argv(infile), perror("dup2: "), close(fd), 1);
	return (free_argv(infile), close(fd), 0);
}

int	heredoc_loop(int *fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_mode == SIGINT)
				return (0);
			break ;
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
	return (1);
}

int	execute_herdoc(t_cmd *cmd)
{
	int		fd[2];
	t_word	*curr;

	curr = cmd->delimiter;
	while (curr)
	{
		set_heredoc_signals();
		if (pipe(fd) == -1)
			return (perror("pipe"), 1);
		if (!heredoc_loop(fd, curr->value))
		{
			if (g_signal_mode == SIGINT)
				return (close(fd[0]), close(fd[1]), 0);
			return (close(fd[0]), close(fd[1]), 1);
		}
		close(fd[1]);
		if (curr->next == NULL)
		{
			if (dup2(fd[0], STDIN_FILENO) == -1)
				return (perror("dup2"), close(fd[0]), 1);
		}
		else
			close(fd[0]);
		curr = curr->next;
	}
	return (1);
}
