/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 13:22:47 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/19 13:38:46 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

	t_env *
	copy_list(t_env *env)
{
	t_env	*copy;
	t_env	*new;
	char	*key;
	char	*value;

	copy = NULL;
	while (env)
	{
		key = ft_strdup(env->key);
		if (env->value)
			value = ft_strdup(env->value);
		else
			value = NULL;
		new = new_var(key, value);
		if (!new)
		{
			free(key);
			free(value);
			free_env(copy);
			return (NULL);
		}
		add_to_env(&copy, new);
		env = env->next;
	}
	return (copy);
}

static void	swap_env_data(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_val;

	tmp_key = a->key;
	tmp_val = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_val;
}

t_env	*sort_list(t_env *head)
{
	t_env	*cur;
	int		swapped;

	if (!head)
		return (NULL);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = head;
		while (cur->next)
		{
			if (ft_strcmp(cur->key, cur->next->key) > 0)
			{
				swap_env_data(cur, cur->next);
				swapped = 1;
			}
			cur = cur->next;
		}
	}
	return (head);
}
