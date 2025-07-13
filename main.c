#include "minishell.h"
#include "string.h"

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	char	*line;
	t_env	*env;
	t_token *tokens;
	t_ast *ast;
	// int i = 0;

	env = get_env(envp);
	while (1)
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
		t_token *head = tokens;
		if (!check_syntax(tokens))
		{
			free_tokens(head);
			free(line);
			continue;
		}
		ast = parse(&tokens);
		if (!ast)
			printf("AST is Null\n");
		else
			expand_word(ast, env);
		execution(ast, env);
		free_tokens(head);
		free(line);
	}
	free_env(env);
	return (0);
}

