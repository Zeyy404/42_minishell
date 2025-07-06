#include "minishell.h"

void assign_builtin(t_builtin *builtins)
{
    builtins[0].name = "echo";
    builtins[0].f = builtin_echo;
    builtins[1].name = "cd";
    builtins[1].f = builtin_cd;
    builtins[2].name = "pwd";
    builtins[2].f = builtin_pwd;
    builtins[3].name = "export";
    builtins[3].f = builtin_export;
    builtins[4].name = "unset";
    builtins[4].f = builtin_unset;
    builtins[5].name = "exit";
    builtins[5].f = builtin_exit;
}
