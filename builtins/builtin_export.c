/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:25:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/20 11:15:22 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env	*sort_list(t_env *head)
{
	t_env	*current;
	char	*tmp_key;
	char	*tmp_value;
	int		swapped;

	if (!head)
		return (NULL);
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current && current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				tmp_key = current->key;
				tmp_value = current->value;
				current->key = current->next->key;
				current->value = current->next->value;
				current->next->key = tmp_key;
				current->next->value = tmp_value;
				swapped = 1;
			}
			current = current->next;
		}
	}
	return (head);
}

static void	print_export(t_env *env)
{
	t_env	*export;
	// int		i;

	
	export = sort_list(env);
	// i = 0;
	while (export)
	{
		if (export->value)
			printf("declare -x %s=\"%s\"\n", export->key, export->value);
		else
			printf("declare -x %s\n", export->key);
		export = export->next;
	}
}

int	builtin_export(t_shell *shell)
{
	int		i;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	i = 1;
	if (shell->ast->cmd.argv[1] == NULL)
	{
		print_export(shell->env);
		return (0);
	}
	while (shell->ast->cmd.argv[i])
	{
		equal_sign = ft_strchr(shell->ast->cmd.argv[i], '=');
		if (equal_sign)
		{
			var_name = ft_substr(shell->ast->cmd.argv[i], 0, equal_sign
					- shell->ast->cmd.argv[i]);
			var_value = ft_strdup(equal_sign + 1);
			if (!is_valid_key(var_name))
			{
				ft_putstr_fd("export: `", 2);
				ft_putstr_fd(var_name, 2);
				ft_putendl_fd("': not a valid identifier", 2);
				free(var_name);
				free(var_value);
				i++;
				continue ;
			}
			env_set(&shell->env, var_name, var_value);
			free(var_name);
			free(var_value);
		}
		else
		{
			if (!is_valid_key(shell->ast->cmd.argv[i]))
			{
				ft_putstr_fd("export: `", 2);
				ft_putstr_fd(shell->ast->cmd.argv[i], 2);
				ft_putendl_fd("': not a valid identifier", 2);
			}
			else if (!env_get(shell->env, shell->ast->cmd.argv[i]))
				env_set(&shell->env, ft_strdup(shell->ast->cmd.argv[i]), NULL);
		}
		i++;
	}
	return (0);
}
