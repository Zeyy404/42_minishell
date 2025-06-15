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
