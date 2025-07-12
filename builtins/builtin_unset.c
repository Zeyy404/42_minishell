/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:48:38 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/12 17:51:52 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_env(t_env **env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(t_ast *ast, t_env **env)
{
	int	i;

	if (!ast || !ast->cmd.argv)
		return (1);
	i = 1;
	while (ast->cmd.argv[i])
	{
		if (is_valid_key(ast->cmd.argv[i]))
			unset_env(env, ast->cmd.argv[i]);
		i++;
	}
	return (0);
}
