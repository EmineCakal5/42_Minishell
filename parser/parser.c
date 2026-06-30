#include "../minishell.h"

int	is_op(t_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

static t_node	*parse_rec(t_token *t)
{
	t_token	*right;
	t_node	*node;

	right = find_right_pipe(t);
	if (!right)
		return (create_node_cmd(t));
	node = malloc(sizeof(t_node));
	if (!node)
	{
		free_tokens(t);
		free_tokens(right);
		return (NULL);
	}
	node->type = LEAF_PIPE;
	node->cmd = NULL;
	node->left = parse_rec(t);
	node->right = parse_rec(right);
	if (!node->left)
	{
		free_ast(node->right);
		free(node);
		return (NULL);
	}
	if (!node->right)
	{
		free_ast(node->left);
		free(node);
		return (NULL);
	}
	return (node);
}

t_node	*parse(t_token *t)
{
	if (!t)
		return (NULL);
	if (check_parser(t))
	{
		free_tokens(t);
		return (NULL);
	}
	return (parse_rec(t));
}

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
		cmd->args[*i] = strdup(t->value);
		if (!cmd->args[*i])
			return (1);
		(*i)++;
	}
	return (0);
}

t_node	*create_node_cmd(t_token *t)
{
	t_node	*node;
	t_cmd	*cmd;
	t_token	*start;
	int		count;
	int		i;

	start = t;
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
	count = argc_counter(t);
	cmd->args = calloc(count + 1, sizeof(char *));
	if (!cmd->args)
		return (cmd_fail(node, cmd, start));
	i = 0;
	while (t && t->type != PIPE)
	{
		if (t->type == WORD)
		{
			if (add_arg(cmd, t, &i))
				return (cmd_fail(node, cmd, start));
			t = t->next;
		}
		else if (is_redirection(t->type))
		{
			if (save_redirection(cmd, t))
				return (cmd_fail(node, cmd, start));
			t = t->next;
			if (t)
				t = t->next;
		}
		else
			t = t->next;
	}
	node->cmd = cmd;
	/* AST kendi kopyalarını tuttuğu için bu segment artık geçici veridir. */
	free_tokens(start);
	return (node);
}

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

void	print_ast(t_node *node, int depth)
{
	int	i;

	if (!node)
		return ;
	for (i = 0; i < depth; i++)
		printf("  ");
	if (node->type == LEAF_PIPE)
	{
		printf("PIPE\n");
		print_ast(node->left, depth + 1);
		print_ast(node->right, depth + 1);
	}
	else if (node->type == LEAD_CMD && node->cmd)
	{
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
}
