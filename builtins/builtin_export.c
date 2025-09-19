/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:25:33 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/19 13:37:25 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	export_error(char *arg, t_shell *shell)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	shell->exit_status = 1;
}

static void	print_export(t_env *env)
{
	t_env	*copy;
	t_env	*sorted;

	copy = copy_list(env);
	sorted = sort_list(copy);
	while (sorted)
	{
		if (sorted->value)
			printf("declare -x %s=\"%s\"\n", sorted->key, sorted->value);
		else
			printf("declare -x %s\n", sorted->key);
		sorted = sorted->next;
	}
	free_env(copy);
}

static void	handle_variable(char *arg, t_shell *shell)
{
	char	*var_name;

	if (!is_valid_key(arg))
		return (export_error(arg, shell));
	if (!env_get(shell->env, arg))
	{
		var_name = ft_strdup(arg);
		env_set(&shell->env, var_name, NULL);
		free(var_name);
	}
}

static void	handle_assignment(char *arg, t_shell *shell)
{
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	equal_sign = ft_strchr(arg, '=');
	var_name = ft_substr(arg, 0, equal_sign - arg);
	var_value = ft_strdup(equal_sign + 1);
	if (!is_valid_key(var_name))
	{
		export_error(arg, shell);
		free(var_name);
		free(var_value);
		return ;
	}
	env_set(&shell->env, var_name, var_value);
	free(var_name);
	free(var_value);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int		i;

	i = 1;
	if (argv[1] == NULL)
	{
		print_export(shell->env);
		return (0);
	}
	while (argv[i])
	{
		if (ft_strchr(argv[i], '='))
			handle_assignment(argv[i], shell);
		else
			handle_variable(argv[i], shell);
		i++;
	}
	return (shell->exit_status);
}
