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
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status;

    if (pipe(pipefd) == -1)
    {
		g_exit_status = 1;
        return ;
    }
    
    pid1 = fork();
    if (pid1 == 0)
    {
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

    pid2 = fork();
    if (pid2 == 0)
    {
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

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    if (waitpid(pid2, &status, 0) == -1)
	{
		g_exit_status = 1;
		return ;
	}
	g_exit_status = status_to_code(status);

}