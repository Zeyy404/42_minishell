/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:25:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/19 16:32:00 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void free_env_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
}

static void	sort_env_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[i];
				arr[i] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export(t_env *env)
{
	char *equal_sign;
	char **env_arr;
	int i;

	env_arr = env_to_char_array(env);
	sort_env_array(env_arr);
	i = 0;
	while (env_arr[i])
	{
		equal_sign = ft_strchr(env_arr[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			printf("declare -x %s=\"%s\"\n", env_arr[i], equal_sign + 1);
			*equal_sign = '=';
		}
		else
			printf("declare -x %s\n", env_arr[i]);
		i++;
	}
	free_env_arr(env_arr);
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
