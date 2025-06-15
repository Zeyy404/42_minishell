/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:40:45 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/24 12:18:36 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int					sign;
	unsigned long long	result;

	sign = 1;
	result = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-')
	{
		sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + ((*nptr) - '0');
		nptr++;
		if (sign < 0 && result > LONG_MAX)
			return (0);
		if (result > LONG_MAX)
			return (-1);
	}
	return (result * sign);
}

/*
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	printf("%d\n", ft_atoi("8565946695946468464431"));
	printf("%d\n", atoi("8565946695946468464431"));
}
*/