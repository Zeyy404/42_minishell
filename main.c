#include "minishell.h"
#include "string.h"

void	print_env(t_env *env)
{
	while(env)
	{
		ft_putstr_fd(env->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(env->value, 1);
		env = env->next;
	}
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->cmd.argv)
		split_free(ast->cmd.argv);
	if (ast->cmd.delimiter)
		free(ast->cmd.delimiter);
	if (ast->cmd.infile)
		free(ast->cmd.infile);
	if (ast->cmd.outfile)
		free(ast->cmd.outfile);
	free(ast);
}

void	free_shell(t_shell	*shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free(shell->ast);
}

void sigint(int sig)
{
    (void)sig;
    ft_putstr_fd("\n", 1);
    rl_on_new_line();
    // rl_replace_line("", 0);
    rl_redisplay();
}

void    set_signals(void)
{
    signal(SIGINT, sigint);
    signal(SIGQUIT, SIG_IGN);
}

#include <stdio.h>

static const char *token_type_str(t_token_type type)
{
    if (type == WORD)       return "WORD";
    if (type == PIPE)       return "PIPE";
    if (type == REDIR_OUT)  return "REDIR_OUT";
    if (type == REDIR_IN)   return "REDIR_IN";
    if (type == APPEND)     return "APPEND";
    if (type == HEREDOC)    return "HEREDOC";
    if (type == AND_AND)    return "AND_AND";
    if (type == OR_OR)      return "OR_OR";
    if (type == AMPERSAND)  return "AMPERSAND";
    if (type == LPAREN)     return "LPAREN";
    if (type == RPAREN)     return "RPAREN";
    return "UNKNOWN";
}

void    print_tokens(t_token *head)
{
    int i = 0;
    while (head)
    {
        printf("token[%d]: type=%s, value=\"%s\", quotes=%d, dquotes=%d\n",
               i,
               token_type_str(head->type),
               head->value ? head->value : "(null)",
               head->quotes,
               head->dquotes);
        head = head->next;
        i++;
    }
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_token *tokens;
	t_shell	shell;
	int exit_status = 0;

	shell.tokens = NULL;
	shell.ast = NULL;
	shell.env = get_env(envp);
	set_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			free_shell(&shell);
			exit(exit_status);
		}
		if (*line)
			add_history(line);
		tokens = tokenize(line);
		// print_tokens(tokens);
		if (!tokens)
		{
			free(line);
			continue;
		}
		
		if (!check_syntax(tokens))
		{
			free_tokens(tokens);
			free(line);
			continue;
		}
		// print_tokens(shell.tokens);
		shell.tokens = tokens;
		// if (!shell.tokens)
		// 	printf("tokens is not working\n");
		// else
		// 	printf("tokens is working\n");
		// print_tokens(shell.tokens);
		shell.ast = parse(&tokens);
	// 	// if (!shell.ast)
	// 	// 	printf("ast is not working\n");
	// 	// else
	// 	// 	printf("ast is working\n");
	// 	// print_ast(shell.ast, 0, "ROOT");
		if (!shell.ast)
			printf("AST is Null\n");
		else
			expand_word(shell.ast, shell.env, exit_status);
		execution(shell.ast, &shell, 0, &exit_status);
	// // 	free(line);
	// 	free_tokens(shell.tokens);
	// 	free_ast(shell.ast);
	}
	// free_env(shell.env);
	return (0);
}
