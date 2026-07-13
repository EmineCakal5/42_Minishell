/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:25 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:07:26 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_redir	*redir_tail(t_redir *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

t_redir	*new_redir(t_type type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->hd_path = NULL;
	redir->expand = 1;
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_redir **head, t_redir *new)
{
	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	redir_tail(*head)->next = new;
}

int	save_redirection(t_cmd *cmd, t_token *redir_token)
{
	t_redir	*redir;
	t_token	*target;

	if (!cmd || !redir_token || !redir_token->next)
		return (1);
	target = redir_token->next;
	if (target->type != WORD)
		return (1);
	redir = new_redir(redir_token->type, target->value);
	if (!redir)
		return (1);
	if (redir_token->type == HEREDOC)
	{
		free(redir->target);
		redir->target = strip_quotes(target->value);
		redir->expand = !target->quoted;
		cmd->heredoc_fd = -1;
	}
	add_redir(&cmd->redirs, redir);
	return (0);
}
