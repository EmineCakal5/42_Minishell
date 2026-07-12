/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:06 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:07 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"

static int	save_std_fds(int std_fds[2])
{
	std_fds[0] = dup(STDIN_FILENO);
	std_fds[1] = dup(STDOUT_FILENO);
	if (std_fds[0] < 0 || std_fds[1] < 0)
	{
		if (std_fds[0] >= 0)
			close(std_fds[0]);
		if (std_fds[1] >= 0)
			close(std_fds[1]);
		return (-1);
	}
	return (0);
}

static void	restore_std_fds(int std_fds[2])
{
	if (std_fds[0] >= 0)
	{
		dup2(std_fds[0], STDIN_FILENO);
		close(std_fds[0]);
	}
	if (std_fds[1] >= 0)
	{
		dup2(std_fds[1], STDOUT_FILENO);
		close(std_fds[1]);
	}
}

int	run_builtin_with_redirs(t_cmd *cmd, char ***envp)
{
	int	std_fds[2];
	int	status;

	std_fds[0] = -1;
	std_fds[1] = -1;
	if (cmd->redirs)
	{
		if (save_std_fds(std_fds) == -1)
			return (1);
		if (apply_redirections(cmd->redirs) == -1)
		{
			restore_std_fds(std_fds);
			if (g_exit_status == 130)
				return (130);
			return (1);
		}
	}
	status = run_builtin(cmd, envp);
	if (cmd->redirs)
		restore_std_fds(std_fds);
	return (status);
}
