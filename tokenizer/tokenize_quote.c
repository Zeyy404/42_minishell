/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:19 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/17 21:02:51 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_separator_char(char c)
{
	return (ft_isspace(c) || ft_isops(c));
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

static int	handle_quote(const char *input, size_t *i, t_word **words)
{
	size_t	start;
	char	*value;
	t_word	*new;
	char	quote;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
		return (ft_putendl_fd("Error: Unclosed quotes", 2), 0);
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (0);
	new = set_quotes(value, quote);
	free(value);
	if (!new)
		return (0);
	add_word(words, new);
	(*i)++;
	return (1);
}

int	token_quotes(const char *input, size_t *i, t_word **words)
{
	while (input[*i] && !is_separator_char(input[*i]))
	{
		if (is_quote(input[*i]))
		{
			if (!handle_quote(input, i, words))
				return (0);
		}
		else
		{
			if (!handle_word(input, i, words))
				return (0);
		}
	}
	return (1);
}
