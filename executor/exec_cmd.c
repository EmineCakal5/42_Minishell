/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:09 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:09 by ecakal           ###   ########.fr       */
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

static void	child_exec(t_cmd *cmd, char *cmd_path, char **envp)
{
	setup_signals_child();
	if (apply_redirections(cmd->redirs) == -1)
		exit(1);
	execve(cmd_path, cmd->args, envp);
	perror("execve");
	free(cmd_path);
	exit(126);
}

static int	fork_and_exec(t_cmd *cmd, char *cmd_path, char **envp)
{
	pid_t	pid;
	int		status;

	ignore_signals();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		setup_signals();
		return (1);
	}
	if (pid == 0)
		child_exec(cmd, cmd_path, envp);
	free(cmd_path);
	waitpid(pid, &status, 0);
	setup_signals();
	return (status_to_code(status));
}

int	exec_cmd(t_cmd *cmd, t_shell *sh)
{
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		if (cmd && cmd->redirs)
			return (run_redirs_only(cmd));
		return (0);
	}
	if (is_builtin(cmd->args[0]))
		return (run_builtin_with_redirs(cmd, sh));
	cmd_path = find_cmd_path(cmd->args[0], sh->env);
	if (!cmd_path)
		return (command_error(cmd->args[0]));
	return (fork_and_exec(cmd, cmd_path, sh->env));
}
