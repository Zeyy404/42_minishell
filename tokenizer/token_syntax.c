/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:27:00 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/28 23:29:26 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_token(t_token *tok)
{
	if (is_logic_op(tok->type))
	{
		printf("syntax error near unexpected token `%s'\n", tok->value);
		return (258);
	}
	if (is_redir(tok->type) && (!tok->next || is_operator(tok->next->type)))
	{
		if (!tok->next)
			printf("syntax error near unexpected token `newline'\n");
		else
			printf("syntax error near unexpected token `%s'\n",
				tok->next->value);
		return (258);
	}
	return (0);
}

static int	check_after_redir(t_token *tok)
{
	if (is_operator(tok->type))
	{
		printf("syntax error near unexpected token `%s'\n", tok->value);
		return (258);
	}
	return (0);
}

static int	check_end_token(t_token *tok)
{
	if (is_logic_op(tok->type) || is_redir(tok->type))
	{
		printf("syntax error near unexpected token `newline'\n");
		return (258);
	}
	return (0);
}

int	check_syntax(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (!prev)
		{
			if (check_token(tokens))
				return (258);
		}
		else if (is_logic_op(prev->type) && check_token(tokens))
			return (258);
		else if (is_redir(prev->type) && check_after_redir(tokens))
			return (258);
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && check_end_token(prev))
		return (258);
	return (0);
}
