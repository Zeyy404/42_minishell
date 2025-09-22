/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:35:59 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/22 19:34:46 by yalkhidi         ###   ########.fr       */
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
	if (ft_strchr(cmd, '/'))
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": Not a directory", 2);
	}
	else
	{
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": command not found", 2);
	}
}

void	exec_child_cmd(t_ast *ast, t_shell *shell)
{
	// printf("exec child\n");
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
	// printf("argv[0]: '%s'\n", argv[0]);
	if (!argv || !argv[0])
		clean_and_exit(argv, NULL, shell, 0);
	cmd_path = find_cmd_path(argv[0], shell->env, &ast->cmd.cmd_not_found);
	// printf("cmd:'%s'\n", cmd_path);	
	if (ast->cmd.cmd_not_found)
	{
		print_cmd_error(argv[0]);
		clean_and_exit(argv, NULL, shell, 127);
	}
	if (!cmd_path)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": No such file or directory", 2);
		clean_and_exit(argv, NULL, shell, 127);
	}
	if (cmd_path)
	{
		if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": is a directory", 2);
			clean_and_exit(argv, NULL, shell, 126);
		}
	}
	env_arr = env_to_char_array(shell->env);

	if (execve(cmd_path, argv, env_arr) == -1)
	{
		
		// printf("execfail errno: %d\n", errno);
		if (errno == ENOENT)
		{
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 127);
		}
		else
		{
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 126);
		}
		if (errno == EISDIR)
		{
			// printf("EISDIR\n");
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 126);
		}
		else if (errno == ENOENT)
		{
			// printf("ENOENT\n");
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 127);
		}
		else if (errno == EACCES)
		{
			// printf("EACCES\n");
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 126);
		}
		else if (errno == ENOTDIR)
		{
			// printf("ENOTDIR\n");
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 126);
		}
		else if (errno == ENOEXEC)
		{
			// printf("ENOEXEC\n");
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 126);
		}
	}
}


