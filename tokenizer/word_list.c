/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 20:13:38 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/17 20:57:09 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_word	*new_word(char *value, t_quote quote_type)
{
	t_word	*word;

	word = malloc(sizeof(t_word));
	if (!word)
		return (NULL);
	word->value = ft_strdup(value);
	if (!word->value)
	{
		free(word);
		return (NULL);
	}
	word->quote_type = quote_type;
	word->next = NULL;
	return (word);
}

void	add_word(t_word **words, t_word *new_word)
{
	t_word	*current;

	if (!words || !new_word)
		return ;
	if (!*words)
	{
		*words = new_word;
		return ;
	}
	current = *words;
	while (current->next)
		current = current->next;
	current->next = new_word;
}

void	free_word_list(t_word *words)
{
	t_word	*tmp;

	while (words)
	{
		tmp = words;
		words = words->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
