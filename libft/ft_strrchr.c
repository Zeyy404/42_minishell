/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:14:14 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/25 00:20:22 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	if (c == '\0')
	{
		return ((char *)s + len);
	}
	while (len--)
	{
		if (s[len] == (char)c)
			return ((char *)s + len);
	}
	return (NULL);
}
