/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:40:12 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/24 17:22:32 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*p_dest;
	const unsigned char	*p_src;

	p_dest = (unsigned char *)dest;
	p_src = (const unsigned char *)src;
	i = 0;
	if (p_dest > p_src && p_dest < (p_src + n))
	{
		while (n--)
		{
			p_dest[n] = p_src[n];
		}
	}
	else
	{
		while (i < n)
		{
			p_dest[i] = p_src[i];
			i++;
		}
	}
	return (dest);
}

/*
#include <stdio.h>

int main()
{
	char	src[10] = "abcdefghi";
	printf("%s", ft_memmove(src + 5, src, 3));
	return (0);	
}
*/