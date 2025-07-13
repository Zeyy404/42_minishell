#include "../minishell.h"

bool	is_builtin(char *cmd)
{
    t_builtin builtins[8];
    int i;

    if (!cmd)
        return (false);
    assign_builtin(builtins);
    i = 0;
    while(builtins[i].name)
    {
        if (!ft_strcmp(cmd, builtins[i].name))
            return (true);
        i++;
    }
    return (false);
}

void	execute_builtins(t_ast *ast, t_env *env, char *cmd)
{
    t_builtin builtins[8];
    int i;

    assign_builtin(builtins);
    i = 0;
    while(builtins[i].name)
    {
        if (!ft_strcmp(cmd, builtins[i].name))
        {
            builtins[i].f(ast, &env);
            break ;
        }
        i++;
    }
}