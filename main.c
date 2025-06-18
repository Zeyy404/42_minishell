#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_data	data;

	data.env = init_env(envp);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (!strcmp(line, "env"))
			print_env(data.env);
		free(line);
	}
	return (0);
}
