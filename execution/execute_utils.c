/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:44:11 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/07/20 07:48:59 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_free(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*concat(char *path, char slash, char *cmd)
{
	char	*concat;
	size_t	leng;
	int		i;
	int		j;

	leng = ft_strlen(path) + ft_strlen(cmd) + 2;
	concat = malloc(leng * sizeof(char));
	if (!concat)
		return (NULL);
	i = 0;
	while (path[i])
	{
		concat[i] = path[i];
		i++;
	}
	concat[i++] = slash;
	j = 0;
	while (cmd[j])
		concat[i++] = cmd[j++];
	concat[i] = '\0';
	return (concat);
}

char	**env_to_char_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**arr;
	int		i;

	tmp = env;
	count = 0;
	i = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		arr[i++] = concat(tmp->key, '=', tmp->value);
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}
