#include "../minishell.h"

bool	ft_isops(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&');
}

bool	ft_isspace(char c)
{
	return (c >= 9 && c <= 13 || c == 32);
}
