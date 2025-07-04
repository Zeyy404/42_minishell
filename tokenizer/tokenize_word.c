/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/27 16:26:34 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	token_word(const char *input, size_t *i, t_token **tokens)
{
	size_t	start;
	char	*value;

	start = *i;
	while (input[*i] && !ft_isops(input[*i]) && !ft_isspace(input[*i]))
		(*i)++;
	if (*i == start)
		return (0);
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (0);
	add_token(tokens, new_token(WORD, value));
	free(value);
	return (1);
}
