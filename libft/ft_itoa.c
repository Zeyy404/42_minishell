/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:40:02 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 15:40:03 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ndigits(long num)
{
	size_t	count;

	count = 0;
	if (num <= 0)
		count++;
	while (num)
	{
		count++;
		num /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*s;
	size_t	len;
	long	num;

	num = n;
	len = ndigits(num);
	s = (char *)malloc((len + 1) * sizeof(char));
	if (!s)
		return (NULL);
	s[len--] = '\0';
	if (num == 0)
		s[0] = '0';
	if (num < 0)
	{
		s[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		s[len--] = (num % 10) + '0';
		num /= 10;
	}
	return (s);
}
