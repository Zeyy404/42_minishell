/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:43:42 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/27 12:19:30 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_outfile(char *file, int append, int *fd)
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

int	redirect_out(t_argv *argv_files, int append)
{
	int		fd;
	int		err;
	int		i;
	char	**outfile;

	outfile = flatten_argv(argv_files);
	if (!outfile)
		return (0);
	err = 0;
	i = 0;
	while (outfile[i])
	{
		err = open_outfile(outfile[i], append, &fd);
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

int	redirect_in(t_argv *argv_files)
{
	int		fd;
	char	**infile;
	int		i;

	infile = flatten_argv(argv_files);
	if (!infile)
		return (0);
	i = 0;
	while (infile[i])
	{
		fd = open(infile[i], O_RDONLY);
		if (fd == -1)
			return (perror(infile[i]), free_argv(infile), 1);
		if (infile[i + 1])
			close(fd);
		i++;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (free_argv(infile), perror("dup2: "), close(fd), 1);
	return (free_argv(infile), close(fd), 0);
}
