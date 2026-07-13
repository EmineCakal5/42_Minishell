/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_prep.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:13 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:14 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	prep_one_heredoc(t_redir *redir, t_shell *sh)
{
	char	*path;
	int		r;

	path = make_heredoc_path();
	if (!path)
		return (sh->status = 1, -1);
	r = open_heredoc(redir, path, sh);
	if (r == -1)
		sh->status = 1;
	else if (r == -2)
		sh->status = 130;
	if (r < 0)
		return (unlink(path), free(path), -1);
	redir->hd_path = path;
	return (0);
}

static int	prep_cmd_heredocs(t_cmd *cmd, t_shell *sh)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC && prep_one_heredoc(redir, sh) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(t_node *node, t_shell *sh)
{
	if (!node)
		return (0);
	if (prepare_heredocs(node->left, sh) == -1)
		return (-1);
	if (prepare_heredocs(node->right, sh) == -1)
		return (-1);
	if (node->type == LEAD_CMD)
		return (prep_cmd_heredocs(node->cmd, sh));
	return (0);
}
