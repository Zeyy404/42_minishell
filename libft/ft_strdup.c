/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:08:57 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/24 14:39:02 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	int		size;
	int		i;
	char	*src_copy;

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

/*
#include <stdio.h>
#include <string.h>

int main()
{
	char *src;
	char *src_copy;

	src = "Hello";
	src_copy = ft_strdup(src);
	printf("%s\n", src_copy);
	if (src_copy[strlen(src_copy) == '\0'])
		printf("Null-terminated\n");
	return (0);
}
*/