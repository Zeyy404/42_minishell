#include "minishell.h"

int	builtin_pwd(t_ast *ast, t_env *env)
{
	char	*cwd;

	(void)env;
	(void)ast;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
