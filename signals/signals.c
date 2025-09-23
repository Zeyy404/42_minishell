/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 12:22:41 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/21 17:19:11 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint(int sig)
{
	(void)sig;
	if (!g_signal_mode)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}



void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_mode = 2;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}
