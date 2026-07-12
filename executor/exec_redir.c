#include "../minishell.h"
#include "executor.h"

static int	open_redir(t_redir *redir)
{
	if (redir->type == REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == HEREDOC)
		return (open(redir->hd_path, O_RDONLY));
	return (-1);
}

int	apply_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		fd = open_redir(redir);
		if (fd == -1)
		{
			perror("minishell");
			return (-1);
		}
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
