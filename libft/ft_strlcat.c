/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:30:50 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/18 18:38:22 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_len;
	size_t	src_len;

	i = 0;
	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	if (size <= dest_len)
	{
		return (size + src_len);
	}
	while (src[i] && dest_len + i < (size - 1))
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest_len + src_len);
}

/*
#include <stdio.h>
#include <string.h>

int main() {
    char dest[20];
    char src[] = " World!";

	strcpy(dest, "Hello");
    printf("Result: %u, dest: %s\n", ft_strlcat(dest, src, 20), dest);
	strcpy(dest, "Hello");
	printf("Result: %lu, dest: %s\n", strlcat(dest, src, 20), dest);

    strcpy(dest, "Hello");
    printf("Result: %u, dest: %s\n", ft_strlcat(dest, src, 10), dest);
	strcpy(dest, "Hello");
	printf("Result: %lu, dest: %s\n", strlcat(dest, src, 10), dest);
    
	strcpy(dest, "Hello");
    printf("Result: %u, dest: %s\n", ft_strlcat(dest, src, 0), dest);
	printf("Result: %lu, dest: %s\n", strlcat(dest, src, 0), dest);
	
    return 0;
}
*/