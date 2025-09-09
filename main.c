#include "minishell.h"
#include "string.h"

int g_child_running = 0;
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


#include <stdio.h>

// Helper to print node type as string
static const char *node_type_str(t_node_type type)
{
    if (type == NODE_CMD) return "CMD";
    if (type == NODE_AND) return "AND";
    if (type == NODE_OR) return "OR";
    if (type == NODE_PIPE) return "PIPE";
    if (type == NODE_GROUP) return "GROUP";
    return "UNKNOWN";
}

// Helper to print indentation
static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("    "); // 4 spaces
}

void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    print_indent(depth);

    // Add marker if root
    if (depth == 0)
        printf("Root Node: %s\n", node_type_str(node->type));
    else
        printf("Node: %s\n", node_type_str(node->type));

    // Print CMD details if needed (your existing code)
    if (node->type == NODE_CMD)
    {
        if (node->cmd.argv)
        {
            print_indent(depth);
            printf("  argv: ");
            for (int i = 0; node->cmd.argv[i]; i++)
                printf("\"%s\" ", node->cmd.argv[i]);
            printf("\n");
        }

        if (node->cmd.infile)
        {
            print_indent(depth);
            printf("  infile: \"%s\"\n", node->cmd.infile);
        }

        if (node->cmd.outfile)
        {
            print_indent(depth);
            printf("  outfile(s): ");
            for (int i = 0; node->cmd.outfile[i]; i++)
                printf("\"%s\" ", node->cmd.outfile[i]);
            printf("\n");
        }

        if (node->cmd.here_doc)
        {
            print_indent(depth);
            printf("  here_doc: yes\n");
            if (node->cmd.delimiter)
            {
                print_indent(depth);
                printf("  delimiters: ");
                for (int i = 0; node->cmd.delimiter[i]; i++)
                    printf("\"%s\" ", node->cmd.delimiter[i]);
                printf("\n");
            }
        }

        print_indent(depth);
        printf("  append: %d\n", node->cmd.append);
    }

    // Recurse into children
    if (node->left)
    {
        print_indent(depth);
        printf("  left:\n");
        print_ast(node->left, depth + 1);
    }

    if (node->right)
    {
        print_indent(depth);
        printf("  right:\n");
        print_ast(node->right, depth + 1);
    }
}


int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_token *tokens;
	t_shell	shell;
	shell.exit_status = 0;

	shell.tokens = NULL;
	shell.ast = NULL;
	shell.env = get_env(envp);
	set_signals();
    // print_env(shell.env);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			free_shell(&shell);
			exit(shell.exit_status);
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
		// print_ast(shell.ast, 0);
		if (!shell.ast)
			printf("AST is Null\n");
		else
			expand_word(shell.ast, shell.env, shell.exit_status);
		// print_ast(shell.ast, 0);
		execution(shell.ast, &shell, 0);
	// // 	free(line);
	// 	free_tokens(shell.tokens);
	// 	free_ast(shell.ast);
	}
	// free_env(shell.env);
	return (0);
}
