/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 02:05:52 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/27 14:29:10 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_argv_list(t_argv *argv)
{
	t_argv	*tmp;

	while (argv)
	{
		tmp = argv->next;
		if (argv->words)
			free_word_list(argv->words);
		free(argv);
		argv = tmp;
	}
}

static void	free_cmd(t_cmd *cmd)
{
	t_redirects	*tmp;

	if (!cmd)
		return ;
	if (cmd->argv)
		free_argv_list(cmd->argv);
	if (cmd->redirections)
	{
		while (cmd->redirections)
		{
			tmp = cmd->redirections->next;
			if (cmd->redirections->files)
				free_argv_list(cmd->redirections->files);
			free(cmd->redirections);
			cmd->redirections = tmp;
		}
	}
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	free_cmd(&node->cmd);
	free(node);
}

void	free_shell(t_shell *shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
}

void	cleanup_iteration(t_shell *shell, char *line)
{
	free_tokens(shell->tokens);
	free_ast(shell->ast);
	shell->tokens = NULL;
	shell->ast = NULL;
	free(line);
	g_signal_mode = -1;
}
