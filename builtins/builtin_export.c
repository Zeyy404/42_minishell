/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:25:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/12 17:47:04 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_export(t_env *env) // env need sorting first!! T-T
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		printf("declare -x %s", cur->key);
		if (cur->value)
			printf("=\"%s\"", cur->value);
		printf("\n");
		cur = cur->next;
	}
}

int	builtin_export(t_ast *ast, t_env **env)
{
	int		i;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	i = 1;
	if (ast->cmd.argv[1] == NULL)
	{
		print_export(*env);
		return (0);
	}
	while (ast->cmd.argv[i])
	{
		equal_sign = ft_strchr(ast->cmd.argv[i], '=');
		if (equal_sign)
		{
			var_name = ft_substr(ast->cmd.argv[i], 0, equal_sign
					- ast->cmd.argv[i]);
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
			env_set(env, var_name, var_value);
			free(var_name);
			free(var_value);
		}
		else
		{
			if (!is_valid_key(ast->cmd.argv[i]))
			{
				ft_putstr_fd("export: `", 2);
				ft_putstr_fd(ast->cmd.argv[i], 2);
				ft_putendl_fd("': not a valid identifier", 2);
			}
			else if (!env_get(*env, ast->cmd.argv[i]))
				env_set(env, ft_strdup(ast->cmd.argv[i]), ft_strdup(""));
		}
		i++;
	}
	return (0);
}
