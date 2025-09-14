/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:19 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 13:35:12 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_separator_char(char c)
{
	return (ft_isspace(c) || ft_isops(c));
}

int is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static t_word	*set_quotes(char *value, char quote)
{
	t_word	*new;

	if (quote == '\'')
		new = new_word(value, Q_SINGLE);
	else if (quote == '"')
		new = new_word(value, Q_DOUBLE);
	else
		new = new_word(value, Q_NONE);
	return (new);
}

int	token_quotes(const char *input, size_t *i, t_word **words)
{
	size_t	start;
	char	*value;
	t_word	*new;
	char	quote;

	while (input[*i] && !is_separator_char(input[*i]))
	{
		if (is_quote(input[*i]))
		{
			quote = input[(*i)++];
			start = *i;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (!input[*i])
				return (0);
			value = ft_substr(input, start, *i - start);
			if (!value)
				return (0);
			new = set_quotes(value, quote);
			free(value);
			if (!new)
				return (0);
			add_word(words, new);
			(*i)++;
		}
		else
		{
			start = *i;
			while (input[*i] && !is_separator_char(input[*i])
				&& !is_quote(input[*i]))
				(*i)++;
			value = ft_substr(input, start, *i - start);
			if (!value)
				return (0);
			new = new_word(value, Q_NONE);
			free(value);
			if (!new)
				return (0);
			add_word(words, new);
		}
	}
	// if (words)
	// 	add_token(tokens, new_token(WORD, NULL, words));
	return (1);
}
