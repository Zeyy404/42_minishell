/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:50 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/03 12:20:41 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_arg(char *arg, t_env *env, int exit_status)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;
	char	*new_arg;

	while (1)
	{
		get_bounds(arg, &start, &end);
		if (start == -1 || end == -1)
			return (arg);
		var_name = ft_substr(arg, start + 1, end - (start + 1));
		value = get_env_value(var_name, env, exit_status);
		free(var_name);
		new_arg = join_before_after(arg, value, start, end);
		free(value);
		free(arg);
		arg = new_arg;
	}
	return (arg);
}

void	expand_argv(char **argv, t_env *env, int *flag, int exit_status)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (flag[i] != 1)
		{
			argv[i] = expand_tilde(argv[i], env, exit_status);
			argv[i] = process_arg(argv[i], env, exit_status);
		}
		i++;
	}
}

void	expand_files(t_ast *ast, t_env *env, int *flag, int exit_status)
{
	int	i;

	i = 0;
	if (ast->cmd.infile)
	{
		ast->cmd.infile = expand_tilde(ast->cmd.infile, env, exit_status);
		ast->cmd.infile = process_arg(ast->cmd.infile, env, exit_status);
	}
	if (ast->cmd.outfile)
	{
		while (ast->cmd.outfile[i])
		{
			if (flag[i] != 1)
			{
				ast->cmd.outfile[i] = expand_tilde(ast->cmd.outfile[i], env,
						exit_status);
				ast->cmd.outfile[i] = process_arg(ast->cmd.outfile[i], env,
						exit_status);
			}
			i++;
		}
	}
}

int	expand_word(t_ast *ast, t_env *env, int exit_status)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && (ast->cmd.argv || ast->cmd.infile
			|| ast->cmd.outfile))
	{
		expand_argv(ast->cmd.argv, env, ast->cmd.quotes, exit_status);
		expand_files(ast, env, ast->cmd.out_quotes, exit_status);
	}
	if (ast->left)
		expand_word(ast->left, env, exit_status);
	if (ast->right)
		expand_word(ast->right, env, exit_status);
	return (1);
}
