/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:50 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/22 12:58:33 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_arg(char *arg, int file, t_env *env, int exit_status)
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
		if (!value && file)
			return (handle_ambiguous_redirect(arg, start, end));
		new_arg = join_before_after(arg, value, start, end);
		free(value);
		free(arg);
		arg = new_arg;
	}
	return (arg);
}

static int	expand_argv(t_argv *argv, int file, t_env *env, int exit_status)
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
				w->value = process_arg(w->value, file, env, exit_status);
				if (!w->value)
					return (0);
			}
			w = w->next;
		}
		curr = curr->next;
	}
	return (1);
}

static int	expand_word_list(t_word *list, int file, t_env *env, int exit_status)
{
	while (list)
	{
		if (list->quote_type != Q_SINGLE)
		{
			list->value = expand_tilde(list->value, env, exit_status);
			list->value = process_arg(list->value, file, env, exit_status);
			if (!list->value)
				return (0);
		}
		list = list->next;
	}
	return (1);
}

static int	expand_files(t_ast *ast, t_env *env, int exit_status)
{
	if (!expand_argv(ast->cmd.infile, 1, env, exit_status))
		return (0);
	if (!expand_argv(ast->cmd.outfile, 1, env, exit_status))
		return (0);
	if (!expand_word_list(ast->cmd.delimiter, 0, env, exit_status))
		return (0);
	return (1);
}

int	expand_word(t_ast *ast, t_env *env, int exit_status)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && (ast->cmd.argv || ast->cmd.infile
			|| ast->cmd.outfile))
	{
		if (!expand_argv(ast->cmd.argv, 0, env, exit_status))
			return (0);
		if (!expand_files(ast, env, exit_status))
			return (0);
	}
	if (ast->left)
		expand_word(ast->left, env, exit_status);
	if (ast->right)
		expand_word(ast->right, env, exit_status);
	return (1);
}
