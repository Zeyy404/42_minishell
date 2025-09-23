/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:35:59 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/23 10:59:55 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_redirects(t_cmd *cmd, t_shell *shell)
{
	int	redirect;

	if (cmd->outfile && execute_redirect_out(cmd))
		clean_and_exit(NULL, NULL, shell, 1);
	if (cmd->infile)
	{
		redirect = execute_redirect_in(cmd);
		if (redirect && g_signal_mode == 2)
			clean_and_exit(NULL, NULL, shell, 1);
		else if (redirect && g_signal_mode == 3)
			clean_and_exit(NULL, NULL, shell, 0);
		else if (redirect)
			clean_and_exit(NULL, NULL, shell, 1);
	}
}

static void	exec_with_path(char **argv, char **env, t_shell *shell)
{
	struct stat	st;
	char		*new_argv[3];

	if (stat(argv[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_error_messages(argv[0], ": is a directory");
		clean_and_exit(argv, env, shell, 126);
	}
	execve(argv[0], argv, env);
	if (errno == ENOEXEC)
	{
		new_argv[0] = "sh";
		new_argv[1] = argv[0];
		new_argv[2] = NULL;
		execve("/bin/sh", new_argv, env);
	}
	if (errno == EACCES || errno == ENOTDIR)
	{
		perror(argv[0]);
		clean_and_exit(argv, env, shell, 126);
	}
	perror(argv[0]);
	clean_and_exit(argv, env, shell, 127);
}

static void	exec_with_env_path(char **argv, t_shell *shell, char **env)
{
	char		*cmd_path;

	cmd_path = find_cmd_path(argv[0], shell->env);
	if (cmd_path == (char *)-1)
	{
		if (check_if_directory(argv[0]))
			clean_and_exit(argv, env, shell, 126);
		print_error_messages(argv[0], ": No such file or directory");
		clean_and_exit(argv, env, shell, 127);
	}
	if (!cmd_path)
	{
		print_error_messages(argv[0], ": command not found");
		clean_and_exit(argv, env, shell, 127);
	}
	if (check_if_directory(cmd_path))
		clean_and_exit(argv, env, shell, 126);
	execve(cmd_path, argv, env);
	perror(argv[0]);
	if (errno == EACCES)
		clean_and_exit(argv, env, shell, 126);
	perror(argv[0]);
	clean_and_exit(argv, env, shell, 127);
}

void	exec_child_cmd(t_ast *ast, t_shell *shell)
{
	char	**argv;
	char	**env;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_redirects(&ast->cmd, shell);
	argv = flatten_argv(ast->cmd.argv);
	if (!argv || !argv[0])
		clean_and_exit(argv, NULL, shell, 0);
	env = env_to_char_array(shell->env);
	if (!env)
		clean_and_exit(argv, NULL, shell, 1);
	if (argv[0][0] == '/' || argv[0][0] == '.')
		exec_with_path(argv, env, shell);
	else
		exec_with_env_path(argv, shell, env);
}
