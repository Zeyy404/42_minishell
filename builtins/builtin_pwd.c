/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:50:35 by zsalih            #+#    #+#             */
/*   Updated: 2025/07/08 15:50:37 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_pwd(t_ast *ast, t_env *env)
{
	char	*cwd;

	(void)env;
	(void)ast;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
