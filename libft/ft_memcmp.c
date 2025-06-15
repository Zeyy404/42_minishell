/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:40:28 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 18:14:53 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*p_s1;
	const unsigned char	*p_s2;

	p_s1 = (const unsigned char *)s1;
	p_s2 = (const unsigned char *)s2;
	i = 0;
	while (i < n && p_s1[i] == p_s2[i])
	{
		i++;
	}
	if (i == n)
		return (0);
	return (p_s1[i] - p_s2[i]);
}
