/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:17 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:07:18 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_IN)
			printf("< %s ", redir->target);
		else if (redir->type == REDIR_OUT)
			printf("> %s ", redir->target);
		else if (redir->type == APPEND)
			printf(">> %s ", redir->target);
		else if (redir->type == HEREDOC)
			printf("<< %s ", redir->target);
		redir = redir->next;
	}
}

static void	print_cmd_node(t_node *node)
{
	int	i;

	printf("CMD: ");
	if (node->cmd->args)
	{
		i = 0;
		while (node->cmd->args[i])
		{
			printf("%s ", node->cmd->args[i]);
			i++;
		}
	}
	print_redirs(node->cmd->redirs);
	printf("\n");
}

void	print_ast(t_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	if (node->type == LEAF_PIPE)
	{
		printf("PIPE\n");
		print_ast(node->left, depth + 1);
		print_ast(node->right, depth + 1);
	}
	else if (node->type == LEAD_CMD && node->cmd)
		print_cmd_node(node);
}
