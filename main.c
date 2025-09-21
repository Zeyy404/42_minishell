#include "minishell.h"
#include "string.h"

int g_signal_mode = 0;
// void	print_env(t_env *env)
// {
// 	while(env)
// 	{
// 		ft_putstr_fd(env->key, 1);
// 		ft_putchar_fd('=', 1);
// 		ft_putendl_fd(env->value, 1);
// 		env = env->next;
// 	}
// }

// void	free_ast(t_ast *ast)
// {
// 	if (!ast)
// 		return ;
// 	free_ast(ast->left);
// 	free_ast(ast->right);
// 	if (ast->cmd.argv)
// 		free_word_list(ast->cmd.argv->words);
// 	if (ast->cmd.delimiter)
// 		free_word_list(ast->cmd.delimiter);
// 	if (ast->cmd.infile)
// 		free_word_list(ast->cmd.infile);
// 	if (ast->cmd.outfile)
// 		free_word_list(ast->cmd.outfile);
// 	free(ast);
// }


#include <stdlib.h>

static void free_words(t_word *w)
{
    t_word *tmp;

    while (w)
    {
        tmp = w->next;
        free(w->value);
        free(w);
        w = tmp;
    }
}

static void free_arg(t_argv *argv)
{
    t_argv *tmp;

    while (argv)
    {
        tmp = argv->next;
        if (argv->words)
            free_words(argv->words);
        free(argv);
        argv = tmp;
    }
}

static void free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;
    if (cmd->argv)
        free_arg(cmd->argv);
    if (cmd->infile)
        free_arg(cmd->infile);
    if (cmd->outfile)
        free_arg(cmd->outfile);
    if (cmd->delimiter)
        free_words(cmd->delimiter);
}

void free_ast(t_ast *node)
{
    if (!node)
        return;
    free_ast(node->left);
    free_ast(node->right);
    free_cmd(&node->cmd);
    free(node);
}


void	free_shell(t_shell	*shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
}

// #include <stdio.h>

// // --- Debug helpers ---

// const char *token_type_str(t_token_type type)
// {
//     switch (type)
//     {
//         case WORD: return "WORD";
//         case PIPE: return "PIPE";
//         case REDIR_OUT: return "REDIR_OUT";
//         case REDIR_IN: return "REDIR_IN";
//         case APPEND: return "APPEND";
//         case HEREDOC: return "HEREDOC";
//         case AND_AND: return "AND_AND";
//         case OR_OR: return "OR_OR";
//         case AMPERSAND: return "AMPERSAND";
//         case LPAREN: return "LPAREN";
//         case RPAREN: return "RPAREN";
//         default: return "UNKNOWN";
//     }
// }

// const char *quote_type_str(t_quote q)
// {
//     switch (q)
//     {
//         case Q_NONE: return "NONE";
//         case Q_SINGLE: return "SINGLE";
//         case Q_DOUBLE: return "DOUBLE";
//         default: return "UNKNOWN";
//     }
// }

// // --- Token printer ---

// void print_tokens(t_token *tokens)
// {
//     int i = 0;
//     while (tokens)
//     {
//         printf("Token %d: type=%s, value='%s'\n",
//                i, token_type_str(tokens->type),
//                tokens->value ? tokens->value : "(null)");

//         t_word *w = tokens->words;
//         int j = 0;
//         while (w)
//         {
//             printf("   Word %d: value='%s', quote=%s\n",
//                    j, w->value ? w->value : "(null)",
//                    quote_type_str(w->quote_type));
//             w = w->next;
//             j++;
//         }

//         tokens = tokens->next;
//         i++;
//     }
// }

// // --- AST printer ---

// static const char *node_type_str(t_node_type type)
// {
//     switch (type)
//     {
//         case NODE_CMD: return "CMD";
//         case NODE_AND: return "AND";
//         case NODE_OR: return "OR";
//         case NODE_PIPE: return "PIPE";
//         case NODE_GROUP: return "GROUP";
//         default: return "UNKNOWN";
//     }
// }

// void print_ast(t_ast *ast, int depth)
// {
//     if (!ast)
//         return;

//     for (int i = 0; i < depth; i++)
//         printf("  "); // indent

//     printf("Node: %s\n", node_type_str(ast->type));

//     if (ast->type == NODE_CMD)
//     {
//         // Print command arguments
//         t_argv *a = ast->cmd.argv;
//         int argn = 0;
//         while (a)
//         {
//             t_word *w = a->words;
//             printf("%*s Arg %d:", depth*2 + 2, "", argn);
//             while (w)
//             {
//                 printf(" '%s'", w->value ? w->value : "(null)");
//                 w = w->next;
//             }
//             printf("\n");
//             a = a->next;
//             argn++;
//         }

//         // Print redirections
//         if (ast->cmd.infile)
//             printf("%*s Infile: '%s'\n", depth*2 + 2, "",
//                    ast->cmd.infile->value ? ast->cmd.infile->value : "(null)");
//         if (ast->cmd.outfile)
//             printf("%*s Outfile: '%s' (append=%d)\n", depth*2 + 2, "",
//                    ast->cmd.outfile->value ? ast->cmd.outfile->value : "(null)",
//                    ast->cmd.append);
//         if (ast->cmd.here_doc && ast->cmd.delimiter)
//             printf("%*s Heredoc delimiter: '%s'\n", depth*2 + 2, "",
//                    ast->cmd.delimiter->value ? ast->cmd.delimiter->value : "(null)");
//     }

//     // Recursive printing of left/right
//     if (ast->left)
//     {
//         for (int i = 0; i < depth; i++)
//             printf("  ");
//         printf(" Left:\n");
//         print_ast(ast->left, depth + 1);
//     }
//     if (ast->right)
//     {
//         for (int i = 0; i < depth; i++)
//             printf("  ");
//         printf(" Right:\n");
//         print_ast(ast->right, depth + 1);
//     }
// }

// int main(int ac, char **av, char **envp)
// {
//     (void)ac;
//     (void)av;
//     t_shell shell;
//     memset(&shell, 0, sizeof(shell));
//     shell.env = get_env(envp);
//     shell.exit_status = 0;
// 	// const char *tests[] = {
//         // "echo Hello World",
//         // "pwd",
//         // "ls -la",
//         // "whoami",
//         // "date",
//         // "hostname",
//         // "uptime",
//         // "ls -la | grep .c",
//         // "cat file.txt | sort | uniq",
//         // "ps aux | grep bash | wc -l",
//         // "echo 'one two three' | tr ' ' '\n' | sort",
//         // "find . -type f | xargs wc -l",
//         // "echo Hello > out.txt",
//         // "cat < file.txt",
//         // "grep TODO file.c >> todo.log",
//         // "echo 'Error' 2> err.log",
//         // "ls &> all_output.log",
//         // "cd /tmp",
//         // "cd -",
//         // "cd ~",
//         // "export TEST_VAR=42",
//         // "env",
//         // "unset TEST_VAR",
//         // "echo $TEST_VAR",
//         // "env",
//     //     "exit 0",
//     //     NULL
//     // };

//     const char *tests[] = {
//         // "mkdir test && cd test",
//         // "rm missing_file || echo 'File not found'",
//         // "false && echo should_not_run",
//         // "true || echo should_not_run",
//         // "false || echo runs",
//         // "(echo grouped; ls)",
//         // "(cd /tmp && echo 'now in tmp')",
//         // "(echo first) | (grep f)",
//         // "(echo a && echo b) || echo c",
//         // "echo 'single quoted string'",
//         // "echo \"double quoted string\"",
//         // "echo \"nested 'quotes' inside\"",
//         // "echo 'nested \"quotes\" inside'",
//         // "echo \"Mix 'of' quotes\" 'and \"more\" quotes'",
//         // "echo $HOME $USER",
//         // "export MYVAR=test && echo $MYVAR",
//         // "echo $? && ls",
//         // "echo ~",
//         // "cd $HOME",
//         // "",
//         // "| echo start",
//         // "echo end |",
//         // "ls &&&& pwd",
//         // "echo \"unterminated string",
//         // "echo 'unterminated string",
//         // "cat <<EOF\nHello\nEOF",
//         // "rm /nonexistent/file",
//         "echo hello",
//         // "ls",
//         NULL
//     };

// 	for(int t = 0; tests[t]; t++)
// 	{
// 		printf("\n=== Test %d: `%s` ===\n", t + 1, tests[t]);
//         shell.tokens = tokenize(tests[t]);
//         if (!shell.tokens)
//         {
//             printf("Tokenization failed!\n");
//             continue;
//         }
//         print_tokens(shell.tokens);
//         if (!check_syntax(shell.tokens))
//         {
//             printf("Syntax error detected!\n");
//             free_tokens(shell.tokens);
//             shell.tokens = NULL;
//             continue ;
//         }
//         t_token *tok_copy = shell.tokens;
//         shell.ast = parse(&tok_copy);
//         if (!shell.ast)
//         {
//             printf("ast failed\n");
//             free_tokens(shell.tokens);
//             continue ; 
//         }
//         if (!expand_word(shell.ast, shell.env, shell.exit_status))
//         {
//             printf("Expansion failed\n");
//             free_tokens(shell.tokens);
//             free_ast(shell.ast);
//             shell.ast = NULL;
//             shell.tokens = NULL;
//             continue ;
//         }
//         print_ast(shell.ast, 0);
//         execution(shell.ast, &shell, 0);
//         free_ast(shell.ast);
//         free_tokens(shell.tokens);
//         shell.ast = NULL;
//         shell.tokens = NULL;
//     }
//     free_env(shell.env);
//     return 0;
// }

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_shell	shell;
    t_token *tokens;

	shell.exit_status = 0;
	shell.tokens = NULL;
	shell.ast = NULL;
	shell.env = get_env(envp);
    // int i = 0;
	while (1)
	{     
        set_signals();
        // printf("exit status = %d\n", shell.exit_status);
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			free_shell(&shell);
			exit(shell.exit_status);
		}
		if (*line)
			add_history(line);
		shell.tokens = tokenize(line);
		if (!shell.tokens)
		{
			free(line);
			continue;
		}
		if (check_syntax(shell.tokens) == 258)
		{
            shell.exit_status = 258;
			free_tokens(shell.tokens);
            shell.tokens = NULL;
			free(line);
			continue;
		}
        tokens = shell.tokens;
        // print_tokens(shell.tokens);
		shell.ast = parse(&tokens);
        // print_ast(shell.ast, 0);
		if (shell.ast)
			expand_word(shell.ast, shell.env, shell.exit_status);
		execution(shell.ast, &shell, 0);
        free_tokens(shell.tokens);
        // free_ast(shell.ast);
        shell.tokens = NULL;
        shell.ast = NULL;
        free(line);
        // i++; 
        g_signal_mode = 0;
	}
	free_env(shell.env);
    // rl_clear_history();
	return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// 	char	*line;
// 	t_token *tokens;
// 	t_shell	shell;
// 	shell.exit_status = 0;

// 	shell.tokens = NULL;
// 	shell.ast = NULL;
// 	shell.env = get_env(envp);
// 	set_signals();
//     // print_env(shell.env);
// 	while (1)
// 	{
// 		line = readline("minishell$ ");
// 		if (!line)
// 		{
// 			ft_putendl_fd("exit", 1);
// 			free_shell(&shell);
// 			exit(shell.exit_status);
// 		}
// 		if (*line)
// 			add_history(line);
// 		tokens = tokenize(line);
// 		// print_tokens(tokens);
// 		if (!tokens)
// 		{
// 			free(line);
// 			printf("debug: syntax error\n");
// 			continue;
// 		}
		
// 		if (!check_syntax(tokens))
// 		{
// 			free_tokens(tokens);
// 			free(line);
// 			continue;
// 		}
// 		// print_tokens(shell.tokens);
// 		shell.tokens = tokens;
// 		// if (!shell.tokens)
// 		// 	printf("tokens is not working\n");
// 		// else
// 		// 	printf("tokens is working\n");
// 		// print_tokens(shell.tokens);
// 		shell.ast = parse(&tokens);
// 	// 	// if (!shell.ast)
// 	// 	// 	printf("ast is not working\n");
// 	// 	// else
// 	// 	// 	printf("ast is working\n");
// 		// print_ast(shell.ast, 0);
// 		if (!shell.ast)
// 			printf("AST is Null\n");
// 		else
// 			expand_word(shell.ast, shell.env, shell.exit_status);
// 		// print_ast(shell.ast, 0);
// 		execution(shell.ast, &shell, 0);
// 	// // 	free(line);
// 	// 	free_tokens(shell.tokens);
// 	// 	free_ast(shell.ast);
// 	}
// 	// free_env(shell.env);
// 	return (0);
// }
