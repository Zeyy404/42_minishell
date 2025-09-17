/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:22:41 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/17 19:16:43 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint(int sig)
{
	(void)sig;
	if (!g_child_running)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signals(void)
{
	signal(SIGINT, sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}
