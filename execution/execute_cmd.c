/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:35:59 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/22 02:40:27 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_and_exit(char **argv, char **env_arr, t_shell *shell,
	int exit_code)
{
	if (argv)
		free_argv(argv);
	if (env_arr)
		free_argv(env_arr);
	if (shell)
		free_shell(shell);
	exit(exit_code);
}

void	print_cmd_error(char *cmd)
{
	if (cmd[0] == '/')
		perror(cmd);
	else
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
	}
}

void	exec_child_cmd(t_ast *ast, t_shell *shell)
{
	char	*cmd_path;
	char	**env_arr;
	char	**argv;
	int		redirect;
	struct stat	st;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ast->cmd.outfile)
	{
		if (execute_redirect_out(&ast->cmd))
			clean_and_exit(NULL, NULL, shell, 1);
	}
	if (ast->cmd.infile)
	{
		redirect = execute_redirect_in(&ast->cmd);
		if ((redirect && g_signal_mode == 2) )
			clean_and_exit(NULL, NULL, shell, 1);
		else if (redirect && g_signal_mode == 3)
			clean_and_exit(NULL, NULL, shell, 0);
		else if (redirect)
			clean_and_exit(NULL, NULL, shell, 1);
	}

	argv = flatten_argv(ast->cmd.argv);

	if (!argv || !argv[0])
		clean_and_exit(argv, NULL, shell, 0);
	cmd_path = find_cmd_path(argv[0], shell->env);
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": Is a directory", 2);
		clean_and_exit(argv, NULL, shell, 126);
	}
	env_arr = env_to_char_array(shell->env);
	if (!cmd_path)
	{
		print_cmd_error(argv[0]);
		clean_and_exit(argv, env_arr, shell, 127);
	}
	execve(cmd_path, argv, env_arr);
	perror(argv[0]);
	clean_and_exit(argv, env_arr, shell, 1);
}
