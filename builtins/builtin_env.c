#include "../minishell.h"

int	builtin_env(t_shell	*shell)
{
	t_env	*tmp;

	// (void)ast;
	tmp = shell->env;
	while (tmp)
	{
		if (tmp->value)
		{
			ft_putstr_fd(tmp->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(tmp->value, 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
