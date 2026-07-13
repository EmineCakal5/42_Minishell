/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:40 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:41 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"

static int	status_to_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static void	pipe_left(t_node *node, int *pipefd, t_shell *sh)
{
	setup_signals_child();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute(node->left, sh));
}

static void	pipe_right(t_node *node, int *pipefd, t_shell *sh)
{
	setup_signals_child();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute(node->right, sh));
}

static int	wait_two(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, NULL, 0);
	if (waitpid(pid2, &status, 0) == -1)
		return (1);
	return (status_to_code(status));
}

int	exec_pipe(t_node *node, t_shell *sh)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		return (1);
	ignore_signals();
	pid1 = fork();
	if (pid1 == 0)
		pipe_left(node, pipefd, sh);
	pid2 = fork();
	if (pid2 == 0)
		pipe_right(node, pipefd, sh);
	close(pipefd[0]);
	close(pipefd[1]);
	status = wait_two(pid1, pid2);
	setup_signals();
	return (status);
}
