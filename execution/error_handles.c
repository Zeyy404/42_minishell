/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 10:58:21 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/27 12:23:00 by yalkhidi         ###   ########.fr       */
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

void	print_error_messages(char *argv, char *message)
{
	ft_putstr_fd(argv, 2);
	ft_putendl_fd(message, 2);
}

int	check_if_directory(char *arg)
{
	struct stat	st;

	if (stat(arg, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_error_messages(arg, ": is a directory");
		return (1);
	}
	return (0);
}

void	close_tmp_fds(int in, int out)
{
	if (in != -1 && out != -1)
	{
		close(in);
		close(out);
	}
}

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
