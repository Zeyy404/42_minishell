#include "minishell.h"

int builtin_echo(t_ast *ast, t_env *env)
{
    int idx = 1;
    int newline = 1;

    (void)env;
    if (ast->cmd.argv[idx] && ft_strncmp(ast->cmd.argv[idx], "-n", 2) == 0)
    {
        newline = 0;
        idx++;
    }
    while (ast->cmd.argv[idx])
    {
        printf("%s", ast->cmd.argv[idx]);
        if (ast->cmd.argv[idx + 1])
            printf(" ");
        idx++;
    }
    if (newline)
        printf("\n");
    return 0;
}
