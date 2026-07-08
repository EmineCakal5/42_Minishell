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

void	exec_pipe(t_node *node, char ***envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	pid_t	old_pgrp;
	int		status;

	if (pipe(pipefd) == -1)
	{
		g_exit_status = 1;
		return ;
	}
	old_pgrp = get_terminal_pgrp();
	pid1 = fork();
	if (pid1 == 0)
	{
		setpgid(0, 0);
		setup_signals_child();
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute(node->left, envp));
	}
	if (pid1 < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		g_exit_status = 1;
		return ;
	}
	setpgid(pid1, pid1);

	pid2 = fork();
	if (pid2 == 0)
	{
		setpgid(0, pid1);
		setup_signals_child();
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute(node->right, envp));
	}
	if (pid2 < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		g_exit_status = 1;
		return ;
	}
	setpgid(pid2, pid1);

	close(pipefd[0]);
	close(pipefd[1]);
	/* Boru hattini tek bir process group'a topluyoruz ve terminali ona
	   veriyoruz, boylece Ctrl+C shell'e degil pipeline'a gider. */
	set_terminal_pgrp(pid1);
	waitpid(pid1, NULL, 0);
	if (waitpid(pid2, &status, 0) == -1)
	{
		set_terminal_pgrp(old_pgrp);
		g_exit_status = 1;
		return ;
	}
	set_terminal_pgrp(old_pgrp);
	g_exit_status = status_to_code(status);

}