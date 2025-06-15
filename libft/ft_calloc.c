/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:06:41 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 16:07:45 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	total_size;

	if (nmemb == 0 || size == 0 || nmemb > SIZE_MAX / size)
		return (NULL);
	total_size = nmemb * size;
	p = (void *)malloc(total_size);
	if (p)
	{
		ft_bzero(p, total_size);
	}
	return (p);
}
