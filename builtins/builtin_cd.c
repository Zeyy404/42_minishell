/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:23 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/20 11:14:25 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_target(char **target, t_ast *ast, t_env **env)
{
	char	*home_dir;
	char	*prev_dir;

	if (ast->cmd.argv[1] == NULL || ft_strcmp(ast->cmd.argv[1], "~") == 0)
	{
		home_dir = env_get(*env, "HOME");
		if (!home_dir)
			return (ft_putendl_fd("cd: HOME not set", 2), 1);
		*target = home_dir;
	}
	else if (ft_strcmp(ast->cmd.argv[1], "-") == 0)
	{
		prev_dir = env_get(*env, "OLDPWD");
		if (!prev_dir)
			return (ft_putendl_fd("cd: OLDPWD not set", 2), 1);
		*target = prev_dir;
		printf("%s\n", *target);
	}
	else
		*target = ast->cmd.argv[1];
	return (0);
}

int	builtin_cd(t_shell *shell)
{
	char	*buf;
	char	*target;

	buf = getcwd(NULL, 0);
	target = NULL;
	if (get_target(&target, shell->ast, &shell->env) != 0)
		return (free(buf), 1);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(target, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (free(buf), 1);
	}
	if (buf)
	{
		env_set(&shell->env, "OLDPWD", buf);
		free(buf);
	}
	env_set(&shell->env, "PWD", getcwd(NULL, 0));
	return (0);
}
