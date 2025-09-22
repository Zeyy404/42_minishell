/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:44:11 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/22 18:54:01 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
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

char	*get_path(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PATH"))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_env *env, int *cmd_not_found)
{
	char	*path;
	char	**split_path;
	int		i;
	char	*joined_path;

	*cmd_not_found = 0;
	if (cmd[0] == '\0')
	{
		printf("cmd is empty\n");
		return (cmd);
	}
	path = NULL;
	if (!access(cmd, X_OK))
		return (cmd);
	path = get_path(env);
	if (!path)
		return (NULL);
	split_path = ft_split(path, ':');
	i = 0;
	while (split_path[i])
	{
		joined_path = concat(split_path[i], '/', cmd);
		if (!access(joined_path, X_OK))
			return (free_argv(split_path), joined_path);
		free(joined_path);
		i++;
	}
	free_argv(split_path);
	*cmd_not_found = 1;
	return (cmd);
}
