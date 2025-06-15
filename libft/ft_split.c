/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:38:57 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 15:39:07 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	skip_delimiters(char const *s, char c, size_t i)
{
	while (s[i] == c)
		i++;
	return (i);
}

size_t	strs_count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		i = skip_delimiters(s, c, i);
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

void	free_split(char **strs, size_t j)
{
	while (j > 0)
		free(strs[--j]);
	free(strs);
}

char	*get_substr(char const *s, char c, size_t *i)
{
	size_t	start;

	start = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	return (ft_substr(s, start, *i - start));
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	count;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	count = strs_count(s, c);
	strs = (char **)malloc((count + 1) * sizeof(char *));
	if (!strs)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		i = skip_delimiters(s, c, i);
		if (s[i])
		{
			strs[j++] = get_substr(s, c, &i);
			if (!strs[j - 1])
				return (free_split(strs, j), NULL);
		}
	}
	strs[j] = NULL;
	return (strs);
}
