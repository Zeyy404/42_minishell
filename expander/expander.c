/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:50 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/17 21:24:36 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_arg(char *arg, t_env *env, int exit_status)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;
	char	*new_arg;

	while (1)
	{
		get_bounds(arg, &start, &end);
		if (start == -1 || end == -1)
			return (arg);
		var_name = ft_substr(arg, start + 1, end - (start + 1));
		value = get_env_value(var_name, env, exit_status);
		free(var_name);
		new_arg = join_before_after(arg, value, start, end);
		free(value);
		free(arg);
		arg = new_arg;
	}
	return (arg);
}

static void	expand_argv(t_argv *argv, t_env *env, int exit_status)
{
	t_argv	*curr;
	t_word	*w;

	curr = argv;
	while (curr)
	{
		w = curr->words;
		while (w)
		{
			if (w->quote_type != Q_SINGLE)
			{
				w->value = expand_tilde(w->value, env, exit_status);
				w->value = process_arg(w->value, env, exit_status);
			}
			w = w->next;
		}
		curr = curr->next;
	}
}

static void	expand_word_list(t_word *list, t_env *env, int exit_status)
{
	while (list)
	{
		if (list->quote_type != Q_SINGLE)
		{
			list->value = expand_tilde(list->value, env, exit_status);
			list->value = process_arg(list->value, env, exit_status);
		}
		list = list->next;
	}
}

static void	expand_files(t_ast *ast, t_env *env, int exit_status)
{
	expand_word_list(ast->cmd.infile, env, exit_status);
	expand_word_list(ast->cmd.outfile, env, exit_status);
	expand_word_list(ast->cmd.delimiter, env, exit_status);
}

int	expand_word(t_ast *ast, t_env *env, int exit_status)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && (ast->cmd.argv || ast->cmd.infile
			|| ast->cmd.outfile))
	{
		expand_argv(ast->cmd.argv, env, exit_status);
		expand_files(ast, env, exit_status);
	}
	if (ast->left)
		expand_word(ast->left, env, exit_status);
	if (ast->right)
		expand_word(ast->right, env, exit_status);
	return (1);
}
