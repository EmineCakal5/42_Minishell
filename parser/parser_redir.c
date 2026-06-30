#include "../minishell.h"

static t_redir	*redir_tail(t_redir *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

/* Redirection kaydını tek parça halinde oluşturuyoruz. */
t_redir	*new_redir(t_type type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = strdup(target);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

/* Liste sırasını koruyarak sona ekliyoruz. */
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

/* Token'dan target'ı alıp cmd içindeki redir listesine yazarız. */
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
	add_redir(&cmd->redirs, redir);
	if (redir_token->type == HEREDOC)
		cmd->heredoc_fd = -1;
	return (0);
}
