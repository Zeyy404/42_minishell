/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_ops.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:44 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/27 16:26:44 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_double_operators(const char *input, size_t *i,
		t_token **tokens)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, new_token(APPEND, ">>"));
		*i += 2;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, new_token(HEREDOC, "<<"));
		*i += 2;
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		add_token(tokens, new_token(AND_AND, "&&"));
		*i += 2;
	}
	else if (input[*i] == '|' && input[*i + 1] == '|')
	{
		add_token(tokens, new_token(OR_OR, "||"));
		*i += 2;
	}
	else
		return (0);
	return (1);
}

static int	handle_single_operators(const char *input, size_t *i,
		t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, new_token(PIPE, "|"));
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, new_token(REDIR_OUT, ">"));
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, new_token(REDIR_IN, "<"));
		(*i)++;
	}
	else if (input[*i] == '&')
	{
		add_token(tokens, new_token(AMPERSAND, "&"));
		(*i)++;
	}
	else
		return (0);
	return (1);
}

int	token_operators(const char *input, size_t *i, t_token **tokens)
{
	if (handle_double_operators(input, i, tokens))
		return (1);
	else if (handle_single_operators(input, i, tokens))
		return (1);
	return (0);
}
