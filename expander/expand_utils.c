/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:54 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/24 08:07:39 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_ambiguous_redirect(char *arg, int start, int end)
{
	char	*var_name;

	var_name = ft_substr(arg, start, end - start);
	ft_putstr_fd(var_name, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
	free(var_name);
	free(arg);
	return (NULL);
}

char	*get_env_value(const char *name, t_env *env, int exit_status)
{
	if (!ft_strcmp((char *)name, "?"))
		return (ft_itoa(exit_status));
	while (env)
	{
		if (!ft_strcmp((char *)name, env->key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	get_bounds(char *arg, int *start, int *end)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1]
			&& (ft_isalpha(arg[i + 1]) || arg[i + 1] == '_'
				|| arg[i + 1] == '?'))
		{
			*start = i;
			if (arg[i + 1] == '?')
			{
				*end = i + 2;
				return ;
			}
			i++;
			*end = i;
			while (arg[*end] && (ft_isalnum(arg[*end]) || arg[*end] == '_'))
				(*end)++;
			return ;
		}
		i++;
	}
	*start = -1;
	*end = -1;
}

char	*join_before_after(char *arg, char *value, int start, int end)
{
	char	*before;
	char	*after;
	char	*joined;
	char	*new;

	before = ft_substr(arg, 0, start);
	after = ft_strdup(&arg[end]);
	joined = ft_strjoin(before, value);
	new = ft_strjoin(joined, after);
	free(before);
	free(after);
	free(joined);
	return (new);
}

char	*expand_tilde(char *arg, t_env *env, int exit_status)
{
	char	*home;
	char	*new_arg;

	if (!arg || arg[0] != '~')
		return (arg);
	if (arg[1] == '\0' || arg[1] == '/' || arg[1] == ' ')
	{
		home = get_env_value("HOME", env, exit_status);
		if (arg[1] == '\0')
			new_arg = ft_strdup(home);
		else
			new_arg = ft_strjoin(home, &arg[1]);
		free(home);
		free(arg);
		return (new_arg);
	}
	return (arg);
}
