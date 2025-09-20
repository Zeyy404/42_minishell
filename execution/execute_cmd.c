/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:35:59 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/20 11:09:10 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_child_one(char **argv, char **env_arr, t_shell *shell,
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

void	one_cmd(t_ast *ast, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	g_child_running = 1;
	if (pid == 0)
		exec_child_cmd(ast, shell);
	else
		wait_update_status(pid, &shell->exit_status);
}

void	exec_child_cmd(t_ast *ast, t_shell *shell)
{
	char	*cmd_path;
	char	**env_arr;
	char	**argv;
	int		redirect;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirect = execute_redirect_in(&ast->cmd);
	if ((redirect && g_child_running == 2) || execute_redirect_out(&ast->cmd))
		free_child_one(NULL, NULL, shell, 1);
	else if (redirect)
		free_child_one(NULL, NULL, shell, 0);
	argv = flatten_argv(ast->cmd.argv);
	if (!argv || !argv[0])
		free_child_one(argv, NULL, shell, 0);
	cmd_path = find_cmd_path(argv[0], shell->env);
	env_arr = env_to_char_array(shell->env);
	if (!cmd_path)
	{
		print_cmd_error(argv[0]);
		free_child_one(argv, env_arr, shell, 127);
	}
	execve(cmd_path, argv, env_arr);
	perror(argv[0]);
	free_child_one(argv, env_arr, shell, 1);
}
