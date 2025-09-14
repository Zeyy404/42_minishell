/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:08:57 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/11 21:42:52 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	int		size;
	int		i;
	char	*src_copy;

	if (!src)
		return (NULL);
	size = 0;
	while (src[size])
	{
		size++;
	}
	src_copy = (char *)malloc((size + 1) * sizeof(char));
	if (src_copy == NULL)
		return (0);
	i = 0;
	while (src[i])
	{
		src_copy[i] = src[i];
		i++;
	}
	src_copy[i] = '\0';
	return (src_copy);
}
