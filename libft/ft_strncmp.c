/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:45:36 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/24 19:10:54 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0' && s2[i] != '\0')
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

/*
#include <stdio.h>
#include <string.h>

int main()
{
  char *s1;
  char *s2;
  int result;
  int result1;

  s1 = "test\200";
  s2 = "test\0";
  result = ft_strncmp(s1, s2, 6);
  result1 = strncmp(s1, s2, 6);
  printf("my func result = %d\n", result);
  printf("orginal func result = %d\n", result1);
  return (0);
}
*/