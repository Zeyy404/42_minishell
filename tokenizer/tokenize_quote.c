#include "../minishell.h"

int	token_quotes(const char *input, size_t *i, t_token **tokens)
{
	char	quote;
	size_t	start;
	char	*value;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] && input[*i] == quote)
	{
		value = ft_substr(input, start, *i - start);
		if (!value)
			return (0);
		add_token(tokens, new_token(WORD, value));
		free(value);
		(*i)++;
	}
	else
		return (0);
	return (1);
}
