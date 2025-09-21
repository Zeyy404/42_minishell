/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flatten_argv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 09:53:25 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/21 19:09:34 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_partial_argv(char **argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	argv_list_size(t_argv *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

static size_t	get_wrods_length(t_word *words)
{
	size_t	len;
	t_word	*curr;

	curr = words;
	len = 0;
	while (curr)
	{
		len += ft_strlen(curr->value);
		curr = curr->next;
	}
	return (len);
}

char	*flatten_word_list(t_word *words)
{
	size_t	total_len;
	t_word	*curr;
	char	*result;
	char	*ptr;
	size_t	len;

	total_len = get_wrods_length(words);
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ptr = result;
	curr = words;
	while (curr)
	{
		len = ft_strlen(curr->value);
		ft_memcpy(ptr, curr->value, len);
		ptr += len;
		curr = curr->next;
	}
	*ptr = '\0';
	return (result);
}

char	**flatten_argv(t_argv *argv_list)
{
	int		size;
	char	**argv;
	t_argv	*curr;
	int		i;

	size = argv_list_size(argv_list);
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	curr = argv_list;
	i = 0;
	while (curr)
	{
		argv[i] = flatten_word_list(curr->words);
		if (!argv[i])
		{
			free_partial_argv(argv, i);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
}
