/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:54 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/07/13 06:45:32 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (s1[i] == 0 && s2[i] == 0)
		return (0);
	while ((s1[i] == s2[i]) && (s1[i] != '\0' || s2[i] != '\0'))
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*get_env_value(const char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp((char *)name, env->key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	get_bounds(char *arg, int *start, int *end)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1]
			&& (ft_isalpha(arg[i + 1]) || arg[i + 1] == '_'))
		{
			*start = i;
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

char	*process_arg(char *arg, t_env *env)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;
	char	*new_arg;

	while (1)
	{
		get_bounds(arg, &start, &end);
		if (start == -1 || end == -1)
			return (arg);
		var_name = ft_substr(arg, start + 1, end - (start + 1));
		value = get_env_value(var_name, env);
		free(var_name);
		new_arg = join_before_after(arg, value, start, end);
		free(value);
		free(arg);
		arg = new_arg;
		return (new_arg);
	}
}

char	*expand_tilde(char *arg, t_env *env)
{
	char	*home;
	char	*new_arg;

	if (!arg || arg[0] != '~')
		return (arg);
	if (arg[1] == '\0' || arg[1] == '/' || arg[1] == ' ' || arg[1] == '$')
	{
		home = get_env_value("HOME", env);
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
