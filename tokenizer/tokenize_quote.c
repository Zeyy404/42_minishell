/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:19 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/27 18:38:58 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*set_quotes(char	*value, char quote)
{
	t_token *new;

	new = new_token(WORD, value);
	if (!new)
		return (NULL);
	if (quote == '\'')
		new->quotes = 1;
	else
		new->dquotes = 1;
	return (new);
}

int	token_quotes(const char *input, size_t *i, t_token **tokens)
{
	char	quote;
	size_t	start;
	char	*value;
	t_token	*new;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] && input[*i] == quote)
	{
		value = ft_substr(input, start, *i - start);
		if (!value)
			return (0);
		new = set_quotes(value, quote);
		if (!new)
			return (0);
		add_token(tokens, new);
		free(value);
		(*i)++;
	}
	else
		return (0);
	return (1);
}
