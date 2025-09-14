/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 13:31:50 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_word(const char *input, size_t *i, t_token **tokens)
{
	t_word	*words;
	size_t	start;
	char	*value;

	words = NULL;
	while (input[*i] && !ft_isops(input[*i]) && !ft_isspace(input[*i])
		&& input[*i] != '(' && input[*i] != ')')
	{
		if (is_quote(input[*i]))
		{
			if (!token_quotes(input, i, &words))
				return (0);
		}
		else
		{
			start = *i;
			while (input[*i] && !ft_isops(input[*i]) && !ft_isspace(input[*i])
				&& input[*i] != '(' && input[*i] != ')' && !is_quote(input[*i]))
				(*i)++;
			value = ft_substr(input, start, *i - start);
			if (!value)
				return (0);
			add_word(&words, new_word(value, Q_NONE));
			free(value);
		}
	}
	if (words)
		add_token(tokens, new_token(WORD, NULL, words));
	return (1);
}
