/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:04:50 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/08/17 16:27:11 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_arg(char *arg, t_env *env)
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
		value = get_env_value(var_name, env);
		free(var_name);
		new_arg = join_before_after(arg, value, start, end);
		free(value);
		free(arg);
		arg = new_arg;
		return (new_arg);
	}
}

void	expand_argv(char **argv, t_env *env)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		argv[i] = expand_tilde(argv[i], env);
		argv[i] = process_arg(argv[i], env);
		i++;
	}
}

void	expand_files(t_ast *ast, t_env *env)
{
	if (ast->cmd.infile)
	{
		ast->cmd.infile= expand_tilde(ast->cmd.infile, env);
		ast->cmd.infile = process_arg(ast->cmd.infile, env);
	}
	if (ast->cmd.outfile)
	{
		ast->cmd.outfile= expand_tilde(ast->cmd.outfile, env);
		ast->cmd.outfile = process_arg(ast->cmd.outfile, env);
	}
	// printf("infile: %s\n", ast->cmd.infile);
	// printf("outfile: %s\n", ast->cmd.outfile);
}

int	expand_word(t_ast *ast, t_env *env)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && (ast->cmd.argv || ast->cmd.infile || ast->cmd.outfile))
	{
		expand_argv(ast->cmd.argv, env);
		expand_files(ast, env);
	}
	if (ast->left)
		expand_word(ast->left, env);
	if (ast->right)
		expand_word(ast->right, env);
	return (1);
}