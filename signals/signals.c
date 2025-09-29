/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:22:41 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/29 09:18:36 by yalkhidi         ###   ########.fr       */
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
		rl_replace_line("", ));
		rl_redisplay();
	}
}

void	set_signals(void)
{
	if (signal(SIGINT, main_handler) == SIG_ERR)
		perror("SIGINT ");
	if (signal(SIGQUIT, main_handler) == SIG_ERR)
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

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
