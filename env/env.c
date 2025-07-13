/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 17:57:37 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/06/16 17:57:56 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*get_env(char **envp)
{
	size_t	i;
	t_env	*env;
	char	*key;
	char	*value;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		key = get_key_value(envp[i], "key");
		value = get_key_value(envp[i], "value");
		add_to_env(&env, new_var(key, value));
		i++;
	}
	return (env);
}

char	*get_key_value(char *envp, const char *code)
{
	size_t	i;

	if (!envp || !code)
		return (NULL);
	i = 0;
	if (envp[i])
	{
		while (envp[i] != '=')
			i++;
		if (!ft_strncmp(code, "key", 3))
			return (ft_substr(envp, 0, i));
		else
		{
			return (ft_substr(envp, i + 1, ft_strlen(envp)));
		}
		i++;
	}
	return (NULL);
}

t_env	*new_var(char *key, char *value)
{
	t_env	*env_var;

	env_var = malloc(sizeof(t_env));
	env_var->key = key;
	env_var->value = value;
	env_var->next = NULL;
	return (env_var);
}

void	add_to_env(t_env **env, t_env *new_var)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new_var;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_var;
}

void	free_env(t_env *env)
{
	t_env *tmp;

	while(env)
	{
		tmp = env->next;
		free(env->value);
		free(env->key);
		free(env);
		env = tmp;
	}
}