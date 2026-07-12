/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:45 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:46 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"

int	execute(t_node *node, char ***envp)
{
	if (!node)
		return (0);
	if (node->type == LEAF_PIPE)
	{
		exec_pipe(node, envp);
		return (g_exit_status);
	}
	else if (node->type == LEAD_CMD)
	{
		return (exec_cmd(node->cmd, envp));
	}
	return (0);
}
