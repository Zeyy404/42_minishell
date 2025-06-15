/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:38:24 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/18 16:56:47 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		size = size - 1;
		while (src[i] && i < size)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i])
	{
		i++;
	}
	return (i);
}
/*
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *dest;
	char *src;
	unsigned int n;
	unsigned int i;

	src = "Hello, World!";
	dest = (char *)malloc(10 * sizeof(char));
	n = 10;
	i = ft_strlcpy(dest, src, n);
	printf("%s\n", dest);
	printf("%d\n", i);
	free(dest);
	return (0);
}
*/