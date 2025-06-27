/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:49 by zsalih            #+#    #+#             */
/*   Updated: 2025/06/27 18:40:05 by zsalih           ###   ########.fr       */
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
	if (input[*i] == '"' || input[*i] == '\'')
		return (token_quotes(input, i, tokens));
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
			return (NULL);
	}
	return (tokens);
}

// // Note: The following code is commented out for testing purposes.
// #include <stdio.h>

// const char *token_type_to_str(t_token_type type)
// {
//     if (type == WORD)        return ("WORD");
//     if (type == PIPE)        return ("PIPE");
//     if (type == REDIR_OUT)   return ("REDIR_OUT");
//     if (type == REDIR_IN)    return ("REDIR_IN");
//     if (type == APPEND)      return ("APPEND");
//     if (type == HEREDOC)     return ("HEREDOC");
//     if (type == AND_AND)     return ("AND_AND");
//     if (type == OR_OR)       return ("OR_OR");
// 	if (type == AMPERSAND)   return ("AMPERSAND");
//     return ("UNKNOWN");
// }

// void print_tokens(t_token *tokens)
// {
//      while (tokens)
//     {
//         printf("Token: Type [%s], Value [%s]\n, Quotes [%d], DQuotes [%d]\n",
//                token_type_to_str(tokens->type), tokens->value, tokens->quotes, tokens->dquotes);
//         tokens = tokens->next;
//     }
// }

// int main(int ac, char **av)
// {
//     t_token *tokens = tokenize(av[1]);
// 	check_syntax(tokens);
//     print_tokens(tokens);
//     free_tokens(tokens);
//     return (0);
// }
