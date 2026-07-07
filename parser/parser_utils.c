#include "../minishell.h"

int	is_redirection(int type)
{
	return (type == REDIR_IN || type == REDIR_OUT 
		|| type == APPEND || type == HEREDOC);
}

int	check_parser(t_token *t)
{
	t_token	*temp;

	temp = t;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			if (temp->prev == NULL || temp->next == NULL)
				return (printf("minishell: syntax error near pipe\n"), 1);
			if (is_op(temp->prev->type) || is_op(temp->next->type))
				return (printf("minishell: syntax error near pipe\n"), 1);
		}
		else if (is_redirection(temp->type))
		{
			if (temp->next == NULL || temp->next->type != WORD)
				return (printf("minishell: syntax error near redirection\n"), 1);
		}
		temp = temp->next;
	}
	return (0);
}

int	argc_counter(t_token *token)
{
	t_token	*head;
	int		arg_count;

	arg_count = 0;
	head = token;
	while (head && head->type != PIPE)
	{
		if (head->type == WORD)
		{
			if (head->prev == NULL || !is_redirection(head->prev->type))
				arg_count++;
		}
		head = head->next;
	}
	return (arg_count);
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

t_token	*go_end(t_token *t)
{
	if (!t)
		return (NULL);
	while (t && t->next)
		t = t->next;
	return (t);
}

t_token	*find_right_pipe(t_token *t)
{
	t_token	*tmp;
	t_token	*right;

	if (!t)
		return (NULL);
	tmp = go_end(t);
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			right = tmp->next;
			if (tmp->prev)
				tmp->prev->next = NULL;
			if (right)
				right->prev = NULL;
			if (tmp->value)
				free(tmp->value);
			free(tmp);
			return (right);
		}
		tmp = tmp->prev;
	}
	return (NULL);
}
