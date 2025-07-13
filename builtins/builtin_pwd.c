/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:35 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/13 12:35:47 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_ast *ast, t_env **env)
{
	char	*cwd;

	(void)env;
	(void)ast;
	cwd = env_get(*env, "PWD");
	if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	return (0);
}
