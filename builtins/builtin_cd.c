/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:23 by zsalih            #+#    #+#             */
/*   Updated: 2025/09/24 21:48:30 by zsalih           ###   ########.fr       */
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

static void	set_oldpwd(t_env **env, char *old_pwd)
{
	char	*pwd;

	if (old_pwd)
	{
		env_set(env, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	else
	{
		pwd = env_get(*env, "PWD");
		if (pwd)
			env_set(env, "OLDPWD", pwd);
	}
}

static void	set_newpwd(t_env **env, char *target)
{
	char	*new_pwd;
	char	*pwd;
	char	*tmp;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		env_set(env, "PWD", new_pwd);
		free(new_pwd);
		return ;
	}
	pwd = env_get(*env, "PWD");
	if (!pwd)
		return ;
	if (target[0] == '/')
		env_set(env, "PWD", target);
	else
	{
		tmp = concat(pwd, '/', target);
		env_set(env, "PWD", tmp);
		free(tmp);
	}
}

static void	print_getcwd_error(void)
{
	ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot"
		"access parent directories: ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*buf;
	char	*target;

	if (!argv)
		return (1);
	buf = getcwd(NULL, 0);
	if (!buf)
		print_getcwd_error();
	target = NULL;
	if (get_target(&target, argv, &shell->env) != 0)
		return (free(buf), 1);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(target, 2);
		perror(" ");
		free(buf);
		return (1);
	}
	set_oldpwd(&shell->env, buf);
	set_newpwd(&shell->env, target);
	return (0);
}
