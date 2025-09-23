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

// void	exec_child_cmd(t_ast *ast, t_shell *shell)
// {
// 	char	*cmd_path;
// 	char	**env_arr;
// 	char	**argv;
// 	int		redirect;
// 	struct stat	st;

// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (ast->cmd.outfile)
// 	{
// 		if (execute_redirect_out(&ast->cmd))
// 			clean_and_exit(NULL, NULL, shell, 1);
// 	}
// 	if (ast->cmd.infile)
// 	{
// 		redirect = execute_redirect_in(&ast->cmd);
// 		if ((redirect && g_signal_mode == 2) )
// 			clean_and_exit(NULL, NULL, shell, 1);
// 		else if (redirect && g_signal_mode == 3)
// 			clean_and_exit(NULL, NULL, shell, 0);
// 		else if (redirect)
// 			clean_and_exit(NULL, NULL, shell, 1);
// 	}

// 	argv = flatten_argv(ast->cmd.argv);

// 	if (!argv || !argv[0])
// 		clean_and_exit(argv, NULL, shell, 0);
// 	cmd_path = find_cmd_path(argv[0], shell->env);
// 	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
// 	{
// 		ft_putstr_fd(argv[0], 2);
// 		ft_putendl_fd(": Is a directory", 2);
// 		clean_and_exit(argv, NULL, shell, 126);
// 	}
// 	env_arr = env_to_char_array(shell->env);
// 	if (!cmd_path)
// 	{
// 		print_cmd_error(argv[0]);
// 		clean_and_exit(argv, env_arr, shell, 127);
// 	}
// 	execve(cmd_path, argv, env_arr);
// 	perror(argv[0]);
// 	clean_and_exit(argv, env_arr, shell, 1);
// }

void	exec_child_cmd(t_ast *ast, t_shell *shell)
{
	char	*cmd_path;
	char	**env_arr;
	char	**argv;
	struct stat	st;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	/* Handle redirects */
	if (ast->cmd.outfile)
	{
		if (execute_redirect_out(&ast->cmd))
			clean_and_exit(NULL, NULL, shell, 1);
	}
	if (ast->cmd.infile)
	{
		int redirect = execute_redirect_in(&ast->cmd);
		if ((redirect && g_signal_mode == 2))
			clean_and_exit(NULL, NULL, shell, 1);
		else if (redirect && g_signal_mode == 3)
			clean_and_exit(NULL, NULL, shell, 0);
		else if (redirect)
			clean_and_exit(NULL, NULL, shell, 1);
	}

	argv = flatten_argv(ast->cmd.argv);
	if (!argv || !argv[0])
		clean_and_exit(argv, NULL, shell, 0);

	env_arr = env_to_char_array(shell->env);
	if (!env_arr)
		clean_and_exit(argv, NULL, shell, 1);

	/* Case 1: absolute or relative path */
	if (argv[0][0] == '/' || argv[0][0] == '.')
	{
		if (stat(argv[0], &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				ft_putstr_fd(argv[0], 2);
				ft_putendl_fd(": is a directory", 2);
				clean_and_exit(argv, env_arr, shell, 126);
			}
			if (access(argv[0], X_OK) == 0)
				execve(argv[0], argv, env_arr);

			/* Permission issue */
			if (errno == EACCES)
			{
				perror(argv[0]);
				clean_and_exit(argv, env_arr, shell, 126);
			}
		}
		/* If stat fails → no such file */
		// printf("argv111111\n");
		perror(argv[0]);
		if (argv[0][0] == '.')
			clean_and_exit(argv, env_arr, shell, 127); //changed
		else if (argv[0][0] == '/')
			clean_and_exit(argv, env_arr, shell, 126); //changed
	}

	/* Case 2: search in PATH */
	// cmd_path = find_cmd_path(argv[0], shell->env);
	cmd_path = find_cmd_path(argv[0], shell->env);

	if (cmd_path == (char *)-1) // PATH not set
	{
		if (ft_strchr(argv[0], '/'))
		{
			/* It has a slash → try as absolute/relative path */
			if (stat(argv[0], &st) == 0)
			{
				if (S_ISDIR(st.st_mode))
				{
					ft_putstr_fd(argv[0], 2);
					ft_putendl_fd(": is a directory", 2);
					clean_and_exit(argv, env_arr, shell, 126);
				}
				if (access(argv[0], X_OK) == 0)
					execve(argv[0], argv, env_arr);

				if (errno == EACCES || errno == ENOTDIR)
				{
					perror(argv[0]);
					clean_and_exit(argv, env_arr, shell, 126);
				}
			}
			perror(argv[0]);
			clean_and_exit(argv, env_arr, shell, 127);
		}
		else
		{
			/* PATH not set + no slash in command */
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": No such file or directory", 2);
			clean_and_exit(argv, env_arr, shell, 127);
		}
	}
	if (!cmd_path) // PATH searched but not found
	{
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		clean_and_exit(argv, env_arr, shell, 127);
	}

	/* Try execve with PATH result */
	execve(cmd_path, argv, env_arr);

	if (errno == EACCES)
	{
		perror(argv[0]);
		clean_and_exit(argv, env_arr, shell, 126);
	}

	perror(argv[0]);
	clean_and_exit(argv, env_arr, shell, 127);
}
