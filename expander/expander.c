#include "../minishell.h"
#include <string.h>
char    *get_env_value(const char *name, t_env *env)
{
    while(env)
    {
        if (!strcmp(name, env->key))
            return (env->value);
        env = env->next;
    }
    return ("");
}

int expand_word(t_ast *ast, t_env *env)
{
    int i;
    int j;
    int start;
    int end;
    char    *var_name;
    char    *var_value;
    char    *before;
    char    *after;
    char    *new;
    char    **argv;

    i = 0;
    if (!ast )
        return 0;
    argv = ast->cmd.argv;
    if (ast->type == NODE_CMD && ast->cmd.argv)
    {
        while(argv[i])
        {
            j = 0;
            while (argv[i][j])
            {
                if (argv[i][j] == '$' && argv[i][j + 1] && (ft_isalpha(argv[i][j + 1]) || argv[i][j + 1] == '_'))                
                {
                    start = j + 1;
                    end = start;
                    while(argv[i][end] && (ft_isalpha(argv[i][end]) || argv[i][end] == '_' || ft_isdigit(argv[i][end])))
                        end++;
                    var_name = ft_substr(argv[i], start, end - start);
                    printf("VAR_NAME: %s\n", var_name);
                    var_value = get_env_value(var_name, env);
                    free(var_name);
                    before = ft_substr(argv[i], 0, j);
                    after = ft_strdup(&argv[i][end]);
                    new = ft_strjoin(before, var_value);
                    free(before);
                    before = ft_strjoin(new, after);
                    free(after);
                    free(new);
                    free(argv[i]);
                    argv[i] = before;
                    j = -1;
                }
                else if (argv[i][j] == '~' && argv[i][j + 1] == '\0')
                {
                    argv[i] = get_env_value("HOME", env);
                    j = -1;
                }
                j++;
            }
            i++;
        }
    }
    if (ast->left)
        expand_word(ast->left, env);
    if (ast->right)
        expand_word(ast->right, env);
    return (1);
}

