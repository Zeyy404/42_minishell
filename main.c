#include "minishell.h"
#include "string.h"

int g_exit_status = 0;

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
int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	int i = 0;
	char	*line;
	t_token *tokens;
	t_shell	shell;

	shell.env = get_env(envp);
	while (i < 1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = tokenize(line);
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
		shell.tokens = tokens;
		shell.ast = parse(&tokens);
		if (!shell.ast)
			printf("AST is Null\n");
		else
			expand_word(shell.ast, shell.env);
		execution(shell.ast, &shell);
		free(line);
		free_tokens(shell.tokens);
		free_ast(shell.ast);
		i++;
	}
	free_env(shell.env);
	return (0);
}
