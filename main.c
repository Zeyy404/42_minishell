/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 02:21:49 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/24 09:00:11 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_signal_mode = -1;

static void	handle_eof(t_shell *shell, char *line)
{
	ft_putendl_fd("exit", 1);
	free(line);
	free_shell(shell);
	exit(shell->exit_status);
}

static int	handle_syntax(t_shell *shell, char *line)
{
	if (check_syntax(shell->tokens) == 258)
	{
		shell->exit_status = 258;
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free(line);
		return (0);
	}
	return (1);
}

static int	handle_ast(t_shell *shell, t_token *tokens, char *line)
{
	shell->ast = parse(&tokens);
	if (!shell->ast)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_ast(shell->ast);
		shell->ast = NULL;
		free(line);
		return (0);
	}
	expand_word(shell->ast, shell->env, shell->exit_status);
	return (1);
}

static void	shell_loop(t_shell *shell)
{
	t_token	*tokens;
	char	*line;

	while (1)
	{
		set_signals();
		line = readline("minishell$ ");
		if (!line)
			handle_eof(shell, line);
		if (*line)
			add_history(line);
		if (g_signal_mode == SIGINT)
		{
			shell->exit_status = 1;
			g_signal_mode = -1;
		}
		shell->tokens = tokenize(line);
		if (!shell->tokens || !handle_syntax(shell, line))
			continue ;
		tokens = shell->tokens;
		if (!handle_ast(shell, tokens, line))
			continue ;
		execution(shell->ast, shell, 0);
		cleanup_iteration(shell, line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell = (t_shell){get_env(envp), NULL, NULL, 0};
	shell_loop(&shell);
	free_env(shell.env);
	return (0);
}
