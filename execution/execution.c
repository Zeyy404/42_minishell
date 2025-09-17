/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yalkhidi <yalkhidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 07:49:18 by yalkhidi          #+#    #+#             */
/*   Updated: 2025/09/17 10:30:31 by yalkhidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_partial_argv(char **argv, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	argv_list_size(t_argv *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

static char	*flatten_word_list(t_word *words)
{
	size_t	total_len;
	t_word	*curr;
	char	*result;
	char	*ptr;
	size_t	len;

	total_len = 0;
	curr = words;
	while (curr)
	{
		total_len += ft_strlen(curr->value);
		curr = curr->next;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ptr = result;
	curr = words;
	while (curr)
	{
		len = ft_strlen(curr->value);
		ft_memcpy(ptr, curr->value, len);
		ptr += len;
		curr = curr->next;
	}
	*ptr = '\0';
	return (result);
}

char	**flatten_argv(t_argv *argv_list)
{
	int		size;
	char	**argv;
	t_argv	*curr;
	int		i;

	size = argv_list_size(argv_list);
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	curr = argv_list;
	i = 0;
	while (curr)
	{
		argv[i] = flatten_word_list(curr->words);
		if (!argv[i])
		{
			free_partial_argv(argv, i);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	argv[i] = NULL;
	return (argv);
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

// void	execution(t_ast *ast, t_shell *shell, int in_child)
// {
// 	char	**argv;

// 	if (!ast)
// 		return ;
// 	if (ast->type == NODE_CMD)
// 	{
// 		argv = flatten_argv(ast->cmd.argv);
// 		if (argv && is_builtin(argv[0], shell))
// 		{
// 			if (in_child)
// 				builtin_child(ast, shell);
// 			else
// 				builtin_parent(ast, shell);
// 		}
// 		else
// 			execute_one_cmd(ast, shell);
// 		free_argv(argv);
// 	}
// 	else if (ast->type == NODE_PIPE)
// 		execute_pipe(ast, shell);
// 	else if (ast->type == NODE_AND || ast->type == NODE_OR)
// 		execute_and_or(ast, shell, in_child);
// 	else if (ast->type == NODE_GROUP)
// 		execute_group(ast, shell, in_child);
// }

void	execution(t_ast *ast, t_shell *shell, int in_child)
{
	char	**argv;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		argv = flatten_argv(ast->cmd.argv);
		if (argv && is_builtin(argv[0], shell))
		{
			free_argv(argv);
			if (in_child)
			{
				// if (ft_strcmp(argv[0], "exit") == 0)
				builtin_child(ast, shell);
			}
			else
			{
				// if (ft_strcmp(argv[0], "exit") == 0)
					// free_argv(argv);
				builtin_parent(ast, shell);	
				// argv = flatten_argv(ast->cmd.argv);
			}
			// argv = flatten_argv(ast->cmd.argv);
			// if (ft_strcmp(argv[0], "exit") != 0)
			// free_argv(argv);
			// printf("done executing builtin\n");
		}
		else
		{
			free_argv(argv);
			execute_one_cmd(ast, shell);
		}
	}
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast, shell);
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
		execute_and_or(ast, shell, in_child);
	else if (ast->type == NODE_GROUP)
		execute_group(ast, shell, in_child);
}

void	execute_one_cmd(t_ast *ast, t_shell *shell)
{
	pid_t	pid;
	printf("fork 1\n");
	pid = fork();
	g_child_running = 1;
	if (pid == 0)
		exec_child_one(ast, shell);
	else
		wait_update_status(pid, &shell->exit_status);
}

void	execute_pipe(t_ast *ast, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	printf("fork 2\n");
	left_pid = fork();
	g_child_running = 1;
	if (left_pid == 0)
		exec_child_pipe_left(ast, shell, fd);
	printf("fork 3\n");
	right_pid = fork();
	g_child_running = 1;
	if (right_pid == 0)
		exec_child_pipe_right(ast, shell, fd);
	close(fd[0]);
	close(fd[1]);
	wait_update_status(left_pid, &shell->exit_status);
	wait_update_status(right_pid, &shell->exit_status);
}

void	execute_and_or(t_ast *ast, t_shell *shell, int in_child)
{
	execution(ast->left, shell, in_child);
	if (ast->type == NODE_AND && shell->exit_status == 0)
		execution(ast->right, shell, in_child);
	else if (ast->type == NODE_OR && shell->exit_status != 0)
		execution(ast->right, shell, in_child);
}

void	execute_group(t_ast *ast, t_shell *shell, int in_child)
{
	pid_t	pid;

	if (!ast || !ast->left)
	{
		shell->exit_status = 1;
		return ;
	}
	printf("fork 4\n");
	pid = fork();
	g_child_running = 1;
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, sigint);
		signal(SIGQUIT, SIG_DFL);
		if (!in_child && ast->cmd.outfile)
		{
			if (execute_redirect_out(&ast->cmd))
			{
				shell->exit_status = 1;
				exit(1);
			}
		}
		if (ast->cmd.infile)
		{
			if (execute_redirect_in(&ast->cmd, &shell->exit_status))
			{
				shell->exit_status = 1;
				exit(1);
			}
		}
		if (ast->cmd.here_doc)
		{
			if (execute_herdoc(&ast->cmd))
				exit(1);
		}
		execution(ast->left, shell, 1);
		exit(shell->exit_status);
	}
	else
		wait_update_status(pid, &shell->exit_status);
}
