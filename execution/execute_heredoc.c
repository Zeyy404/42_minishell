/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:08:44 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/27 14:25:41 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	heredoc_loop(int *fd, char *delimiter, t_shell *shell)
{
	char	buff[1024];
	ssize_t	n;
	char	*line;

	while (1)
	{
		write(1, "> ", 2);
		n = read(STDIN_FILENO, buff, sizeof(buff) - 1);
		if (n <= 0)
		{
			if (g_signal_mode == SIGINT)
				return (close(fd[0]), close(fd[1]), 0);
			break ;
		}
		buff[n] = '\0';
		if (n > 0 && buff[n - 1] == '\n')
			buff[n - 1] = '\0';
		if (ft_strcmp(buff, delimiter) == 0)
			break ;
		line = process_arg(ft_strdup(buff), 0, shell->env, shell->exit_status);
		ft_putendl_fd(line, fd[1]);
		if (!line)
			free(line);
	}
	return (1);
}

static int	heredoc_redir(t_redirects *redir, int fd[2],
	int *last_fd, t_shell *shell)
{
	if (redir->type == HEREDOC)
	{
		if (pipe(fd) == -1)
			return (perror("pipe"), 1);
		set_heredoc_signals();
		if (!heredoc_loop(fd, redir->files->words->value, shell))
		{
			if (*last_fd != -1)
				close(*last_fd);
			return (0);
		}
		close(fd[1]);
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = fd[0];
	}
	return (0);
}

int	heredoc(t_cmd *cmd, t_shell *shell)
{
	t_redirects	*redir;
	int			fd[2];
	int			last_fd;

	last_fd = -1;
	redir = cmd->redirections;
	while (redir)
	{
		heredoc_redir(redir, fd, &last_fd, shell);
		redir = redir->next;
	}
	if (last_fd != -1)
	{
		if (dup2(last_fd, STDIN_FILENO) == -1)
			return (perror("dup2"), 1);
		close(last_fd);
	}
	return (1);
}
