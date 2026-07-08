#include "../minishell.h"
#include "executor.h"

static int	status_to_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

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

static int	run_builtin_with_redirs(t_cmd *cmd, char ***envp)
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
	fflush(NULL);
	if (cmd->redirs)
		restore_std_fds(std_fds);
	return (status);
}

int	exec_cmd(t_cmd *cmd, char ***envp)
{
	pid_t	pid;
	pid_t	old_pgrp;
	char	*cmd_path;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		return (run_builtin_with_redirs(cmd, envp));
	}
	cmd_path = find_cmd_path(cmd->args[0], *envp);
	if (!cmd_path)
	{
		printf("%s: command not found\n", cmd->args[0]);
		return (127);
	}
	old_pgrp = get_terminal_pgrp();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		setpgid(0, 0);
		setup_signals_child();
		if (apply_redirections(cmd->redirs) == -1)
		{
			if (g_exit_status == 130)
				exit(130);
			exit(1);
		}
		execve(cmd_path, cmd->args, *envp);
		perror("execve");
		free(cmd_path);
		exit(126);
	}
	/* Komutu kendi process group'una tasiyip terminali ona devrediyoruz,
	   boylece Ctrl+C sadece o gruba gider, shell'in kendisi etkilenmez. */
	setpgid(pid, pid);
	set_terminal_pgrp(pid);
	if (waitpid(pid, &status, 0) == -1)
	{
		set_terminal_pgrp(old_pgrp);
		free(cmd_path);
		return (1);
	}
	set_terminal_pgrp(old_pgrp);
	free(cmd_path);
	return (status_to_code(status));
}