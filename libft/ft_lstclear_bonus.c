/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 17:24:57 by zsalih            #+#    #+#             */
/*   Updated: 2024/12/22 18:15:19 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*next_lst;

	if (!lst || !del || !*lst)
		return ;
	temp = *lst;
	while (temp)
	{
		next_lst = temp->next;
		del(temp->content);
		free(temp);
		temp = next_lst;
	}
	*lst = NULL;
}
