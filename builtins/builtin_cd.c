/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:23 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/22 12:22:31 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_target(char **target, char **argv, t_env **env)
{
	char	*home_dir;
	char	*prev_dir;

	if (argv[1] == NULL || ft_strcmp(argv[1], "~") == 0)
	{
		home_dir = env_get(*env, "HOME");
		if (!home_dir)
			return (ft_putendl_fd("cd: HOME not set", 2), 1);
		*target = home_dir;
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		prev_dir = env_get(*env, "OLDPWD");
		if (!prev_dir)
			return (ft_putendl_fd("cd: OLDPWD not set", 2), 1);
		*target = prev_dir;
		printf("%s\n", *target);
	}
	else
		*target = argv[1];
	return (0);
}

static void	update_pwd(t_env **env, char *old_pwd)
{
	char	*new_pwd;

	if (old_pwd)
	{
		env_set(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		env_set(env, "PWD", new_pwd);
		free(new_pwd);
	}
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*buf;
	char	*target;

	if (!argv)
		return (1);
	buf = getcwd(NULL, 0);
	target = NULL;
	if (get_target(&target, argv, &shell->env) != 0)
		return (free(buf), 1);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(target, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(buf);
		// free_argv(argv);
		return(1);
		// return (free(buf), free_argv(argv), 1);
	}
	update_pwd(&shell->env, buf);
	return (0);
}
