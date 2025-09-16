/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:26:49 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/14 13:31:04 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	token_consume_word_or_quote( const char *input, size_t *i, t_token **tokens)
// {
// 	size_t start;
// 	char *value;
// 	char *part;
// 	char quote;
// 	char *tmp;

// 	value = ft_strdup("");
// 	if (!value)
// 		return (0);
// 	while(input[*i] && !ft_isspace(input[*i]) && !ft_isops(input[*i]) && input[*i] != '(' && input[*i] != ')')
// 	{
// 		if (input[*i] == '"' || input[*i] == '\'')
// 		{
// 			quote = input[(*i)++];
// 			printf("quote: %c\n", quote);
// 			printf("input[%ld]: %c\n",*i, input[*i]);
// 			start = *i;
// 			while(input[*i] && input[*i] != quote)
// 				(*i)++;
// 			part = ft_substr(input, start, *i - start);
// 			if (!part)
// 				return (0);
// 			tmp = value;
// 			value = ft_strjoin(value, part);
// 			free(part);
// 			free(tmp);
// 			if (input[*i] == quote)
// 				(*i)++;
// 		}
// 		else
// 		{
// 			start = *i;
// 			while (input[*i] && !ft_isspace(input[*i]) && !ft_isops(input[*i]) && input[*i] != '(' && input[*i] != ')' && input[*i] != '"' && input[*i] != '\'')
// 				(*i)++;
// 			part = ft_substr(input, start, *i - start);
// 			tmp = value;
// 			value = ft_strjoin(value, part);
// 			free(tmp);
// 			free(part);
// 		}
// 	}
// 	add_token(tokens, new_token(WORD, value));
// 	free(value);
// 	return (1);
// }

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
	// if (input[*i] == '"' || input[*i] == '\'')
	// 	return (token_quotes(input, i, tokens));
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

// const char *quote_type_to_str(t_quote type)
// {
//     if (type == Q_NONE)     return ("NO QUOTES");
//     if (type == Q_SINGLE)   return ("SINGLE QUOTES");
//     if (type == Q_DOUBLE)   return ("DOUBLE QUOTES");
//     return ("UNKNOWN");
// }

// void print_tokens(t_token *tokens)
// {
//      while (tokens)
//     {
// 		if (tokens->type != WORD)
//         	printf("Token: Type [%s], Value [%s]\n",
//                	token_type_to_str(tokens->type), tokens->value);
// 		else if (tokens->type == WORD)
// 		{
//     		t_word *w = tokens->words;
//     	while (w)
//     	{
//         	printf("Words [%s], Quotes [%s]\n",
//             	   w->value, quote_type_to_str(w->quote_type));
//         	w = w->next;
//     	}
// 		}
//         tokens = tokens->next;
//     }
// }

// int main(int ac, char **av)
// {
// 	(void)ac;
//     t_token *tokens = tokenize(av[1]);
// 	check_syntax(tokens);
//     print_tokens(tokens);
//     free_tokens(tokens);
//     return (0);
// }
