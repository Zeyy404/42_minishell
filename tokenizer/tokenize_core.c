/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:49 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/17 20:51:23 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] == '\0')
		return (0);
	return (1);
}

static int	dispatch_token(const char *input, size_t *i, t_token **tokens)
{
	if (input[*i] == '(' || input[*i] == ')')
	{
		if (input[*i] == '(')
			add_token(tokens, new_token(LPAREN, "(", NULL));
		else
			add_token(tokens, new_token(RPAREN, ")", NULL));
		(*i)++;
		return (1);
	}
	else if (ft_isops(input[*i]))
		return (token_operators(input, i, tokens));
	else
		return (token_word(input, i, tokens));
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	size_t	i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (!skip_whitespace(input, &i))
			break ;
		if (!dispatch_token(input, &i, &tokens))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
