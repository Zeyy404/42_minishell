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
size_t	env_len(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		i++;
	}
	return (i);
}

char	**init_env(char **envp)
{
	char	**copy_env;
	size_t	i;
	size_t	len;

	len = env_len(envp);
	copy_env = malloc((len + 1) * sizeof(char **));
	if (!copy_env)
	{
		write(2, "Error\n allocating env copy\n", 28);
		exit(1);
	}
	i = 0;
	while (i < len)
	{
		copy_env[i] = ft_strdup(envp[i]);
		i++;
	}
	copy_env[i] = NULL;
	return (copy_env);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

void	print_env(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		ft_putstr(envp[i]);
		ft_putstr("\n");
		i++;
	}
}