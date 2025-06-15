/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:36:51 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 15:37:36 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	len;
	size_t	start;
	size_t	end;

	if (!s1)
		return (NULL);
	if (!set || set[0] == '\0')
		return (ft_strdup(s1));
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	if (end > 0)
		end--;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	if (start > end)
		return (ft_strdup(""));
	len = end - start + 1;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1 + start, len + 1);
	return (str);
}
