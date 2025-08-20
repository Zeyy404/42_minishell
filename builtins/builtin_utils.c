/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih < zsalih@student.42abudhabi.ae>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:45 by zsalih            #+#    #+#             */
/*   Updated: 2025/08/21 00:46:38 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	assign_builtin(t_builtin *builtins, t_shell *shell)
{
	builtins->shell = shell;
	builtins[0].name = "echo";
	builtins[0].f = builtin_echo;
	builtins[1].name = "cd";
	builtins[1].f = builtin_cd;
	builtins[2].name = "pwd";
	builtins[2].f = builtin_pwd;
	builtins[3].name = "export";
	builtins[3].f = builtin_export;
	builtins[4].name = "unset";
	builtins[4].f = builtin_unset;
	builtins[5].name = "exit";
	builtins[5].f = builtin_exit;
	builtins[6].name = "env";
	builtins[6].f = builtin_env;
	builtins[7].name = NULL;
	builtins[7].f = NULL;
}

void	env_set(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new = new_var(key, value);
	add_to_env(env, new);
}

char	*env_get(t_env *env, const char *key)
{
	if (!env)
		return (NULL);
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	is_valid_key(const char *str)
{
	int	i;

	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
