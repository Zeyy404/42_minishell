/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:22:41 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/24 09:41:38 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	main_handler(int sig)
{
	if (g_signal_mode == 0)
		return ;
	g_signal_mode = sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signals(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = &main_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) < 0)
		perror("SIGINT ");
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
		perror("SIGQUIT ");
}

void	sigint_heredoc(int sig)
{
	if (sig == SIGINT)
		g_signal_mode = sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	set_heredoc_signals(void)
{
	if (signal(SIGINT, sigint_heredoc) == SIG_ERR)
		perror("SIGINT ");
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		perror("SIGQUIT ");
}

void	set_non_interactive_signals(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("SIGINT ");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("SIGQUIT ");
}
