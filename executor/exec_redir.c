#include "../minishell.h"
#include "executor.h"

int	apply_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir-> type == REDIR_IN)
			fd = open(redir->target, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == APPEND)
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if ( redir->type == HEREDOC)
			fd = open_heredoc(redir->target);
		else
			fd = -1;
		
		if (fd == -2)
			return (-1);//heredoc Ctrl+C ile iptal edildi, sessizce çık
		if (fd == -1)
		{
			perror("minishell");//dosya açma hatası
			return (-1);
		}

		//giriş-çıkış yönlendirme
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		
		close(fd);
		redir = redir->next;
	}
	return (0);
}