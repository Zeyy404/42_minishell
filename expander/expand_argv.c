/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 20:48:29 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/23 21:54:43 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	should_remove_var(const char *str, t_env *env)
{
	char	*val;

	if (!str || str[0] != '$')
		return (0);
	if (str[1] == '?' || str[1] == '\0')
		return (0);
	val = env_get(env, str + 1);
	if (!val)
		return (1);
	return (0);
}

static void	expand_word_node(t_word *w, int file, t_env *env, int exit_status)
{
	if (w->quote_type != Q_SINGLE)
	{
		w->value = expand_tilde(w->value, env, exit_status);
		w->value = process_arg(w->value, file, env, exit_status);
		if (!w->value)
			w->value = ft_strdup("");
	}
}

static void	remove_word(t_argv *curr, t_word *w, t_word *prev)
{
	t_word	*next;

	next = w->next;
	if (prev)
		prev->next = next;
	else
		curr->words = next;
	free(w->value);
	free(w);
}

static void	expand_words_in_argv(t_argv *curr, int file, t_env *env,
		int exit_status)
{
	t_word	*w;
	t_word	*prev;
	t_word	*next;

	w = curr->words;
	prev = NULL;
	while (w)
	{
		next = w->next;
		if (!file && should_remove_var(w->value, env)
			&& w->quote_type == Q_NONE)
			remove_word(curr, w, prev);
		else
		{
			expand_word_node(w, file, env, exit_status);
			prev = w;
		}
		w = next;
	}
}

int	expand_argv(t_argv **argv, int file, t_env *env, int exit_status)
{
	t_argv	*curr;
	t_argv	*prev;
	t_argv	*next_argv;

	curr = *argv;
	prev = NULL;
	while (curr)
	{
		expand_words_in_argv(curr, file, env, exit_status);
		if (!curr->words)
		{
			next_argv = curr->next;
			if (prev)
				prev->next = next_argv;
			else
				*argv = next_argv;
			free(curr);
			curr = next_argv;
			continue ;
		}
		prev = curr;
		curr = curr->next;
	}
	return (1);
}
