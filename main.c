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

void print_ast(t_ast *ast, int depth, char *label)
{
	if (!ast)
		return;

	// Indentation for pretty printing
	for (int i = 0; i < depth; i++)
		printf("  ");

	// Print the label indicating whether this is left or right child (or root)
	if (label)
		printf("[%s] ", label);

	// Print node type and details
	switch (ast->type)
	{
		case NODE_CMD:
			printf("NODE_CMD: ");
			if (ast->cmd.argv)
			{
				for (int i = 0; ast->cmd.argv[i]; i++)
					printf("%s ", ast->cmd.argv[i]);
			}
			printf("\n");
			break;
		case NODE_PIPE:
			printf("NODE_PIPE\n");
			break;
		case NODE_AND:
			printf("NODE_AND (&&)\n");
			break;
		case NODE_OR:
			printf("NODE_OR (||)\n");
			break;
		case NODE_GROUP:
			printf("NODE_GROUP\n");
			break;
		default:
			printf("UNKNOWN NODE TYPE %d\n", ast->type);
	}

	// Recursively print left and right subtrees, with labels
	print_ast(ast->left, depth + 1, "L");
	print_ast(ast->right, depth + 1, "R");
}

const char *token_type_str(int type)
{
    if (type == LPAREN) return "LPAREN";
    if (type == RPAREN) return "RPAREN";
    if (type == WORD) return "WORD";
    if (type == PIPE) return "PIPE";
    if (type == AND_AND) return "AND_AND";
    if (type == OR_OR) return "OR_OR";
    if (type == REDIR_IN) return "REDIR_IN";
    if (type == REDIR_OUT) return "REDIR_OUT";
    if (type == APPEND) return "APPEND";
    if (type == HEREDOC) return "HEREDOC";
    // Add other token types you have here
    return "UNKNOWN";
}

void print_tokens(t_token *tokens)
{
    t_token *tmp = tokens;
    printf("Tokens:\n");
    while (tmp)
    {
        printf("  [%s] \"%s\"\n", token_type_str(tmp->type), tmp->value);
        tmp = tmp->next;
    }
}


int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_token *tokens;
	t_shell	shell;

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
			exit(g_exit_status);
		}
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
		if (!shell.tokens)
			printf("tokens is not working\n");
		else
			printf("tokens is working\n");
		print_tokens(shell.tokens);
		shell.ast = parse(&tokens);
		if (!shell.ast)
			printf("ast is not working\n");
		else
			printf("ast is working\n");
		print_ast(shell.ast, 0, "ROOT");
		if (!shell.ast)
			printf("AST is Null\n");
		else
			expand_word(shell.ast, shell.env);
		execution(shell.ast, &shell);
		free(line);
		free_tokens(shell.tokens);
		free_ast(shell.ast);
	}
	free_env(shell.env);
	return (0);
}
