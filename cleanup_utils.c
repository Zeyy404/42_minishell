/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 02:05:52 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/22 12:19:35 by zsalih           ###   ########.fr       */
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
	if (!cmd)
		return ;
	if (cmd->argv)
		free_argv_list(cmd->argv);
	if (cmd->infile)
		free_argv_list(cmd->infile);
	if (cmd->outfile)
		free_argv_list(cmd->outfile);
	if (cmd->delimiter)
		free_word_list(cmd->delimiter);
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