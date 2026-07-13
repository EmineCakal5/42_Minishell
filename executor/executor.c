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

int	execute(t_node *node, t_shell *sh)
{
	if (!node)
		return (0);
	if (node->type == LEAF_PIPE)
		return (exec_pipe(node, sh));
	else if (node->type == LEAD_CMD)
		return (exec_cmd(node->cmd, sh));
	return (0);
}
