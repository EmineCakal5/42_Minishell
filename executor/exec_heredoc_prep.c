#include "../minishell.h"
#include "executor.h"

static char	*make_heredoc_path(void)
{
	char	*path;

	path = malloc(64);
	if (!path)
		return (NULL);
	if (build_heredoc_name(path, 64) == -1)
		return (free(path), NULL);
	return (path);
}

static int	prep_cmd_heredocs(t_cmd *cmd)
{
	t_redir	*redir;
	char	*path;
	int		r;

	if (!cmd)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			path = make_heredoc_path();
			if (!path)
				return (g_exit_status = 1, -1);
			r = open_heredoc(redir->target, path);
			if (r == -1)
				g_exit_status = 1;
			if (r < 0)
				return (unlink(path), free(path), -1);
			redir->hd_path = path;
		}
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(t_node *node)
{
	if (!node)
		return (0);
	if (prepare_heredocs(node->left) == -1)
		return (-1);
	if (prepare_heredocs(node->right) == -1)
		return (-1);
	if (node->type == LEAD_CMD)
		return (prep_cmd_heredocs(node->cmd));
	return (0);
}
