#include "../minishell.h"

static t_node	*cmd_fail(t_node *node, t_cmd *cmd, t_token *start)
{
	free_cmd_(cmd);
	free_tokens(start);
	free(node);
	return (NULL);
}

static int	add_arg(t_cmd *cmd, t_token *t, int *i)
{
	if (!(t->prev && is_redirection(t->prev->type)))
	{
		cmd->args[*i] = ft_strdup(t->value);
		if (!cmd->args[*i])
			return (1);
		(*i)++;
	}
	return (0);
}

static t_node	*init_cmd_node(t_cmd **cmd_out, t_token *start)
{
	t_node	*node;
	t_cmd	*cmd;

	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_tokens(start);
		return (NULL);
	}
	node->type = LEAD_CMD;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	cmd = create_cmd();
	if (!cmd)
	{
		free(node);
		free_tokens(start);
		return (NULL);
	}
	*cmd_out = cmd;
	return (node);
}

static int	fill_cmd_args(t_cmd *cmd, t_token *t)
{
	int	i;

	i = 0;
	while (t && t->type != PIPE)
	{
		if (t->type == WORD)
		{
			if (add_arg(cmd, t, &i))
				return (1);
			t = t->next;
		}
		else if (is_redirection(t->type))
		{
			if (save_redirection(cmd, t))
				return (1);
			t = t->next;
			if (t)
				t = t->next;
		}
		else
			t = t->next;
	}
	return (0);
}

t_node	*create_node_cmd(t_token *t)
{
	t_node	*node;
	t_cmd	*cmd;
	t_token	*start;
	int		count;

	start = t;
	node = init_cmd_node(&cmd, start);
	if (!node)
		return (NULL);
	count = argc_counter(t);
	cmd->args = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->args)
		return (cmd_fail(node, cmd, start));
	if (fill_cmd_args(cmd, t))
		return (cmd_fail(node, cmd, start));
	node->cmd = cmd;
	free_tokens(start);
	return (node);
}
