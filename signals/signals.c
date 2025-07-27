#include "../minishell.h"

void    sigint(int sig)
{
    (void)sig;

    ft_putstr_fd("\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    set_signals(void)
{
    signal(SIGINT, sigint);
    signal(SIGQUIT, SIG_IGN);
}
