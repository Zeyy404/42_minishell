/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:23 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/08 16:13:30 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

int	builtin_cd(t_ast *ast, t_env *env)
{
	char	*home;

	if (ast->cmd.argv[1] == NULL)
	{
		home = getenv("HOME");
		if (!home)
			return (2);
		else
			chdir(home);
	}
	if (ast->cmd.argv[2] != NULL)
	{
		ft_putendl_fd("cd: too many arguments\n", 2);
		return (1);
	}
	if (!is_directory(ast->cmd.argv[1]))
	{
		ft_putendl_fd("cd: ", 2);
        ft_putendl_fd(ast->cmd.argv[1], 2);
        ft_putendl_fd(": Not a directory\n", 2);
		return (1);
	}
	if (chdir(ast->cmd.argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
