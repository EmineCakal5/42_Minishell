#include "../minishell.h"

int	is_op(t_type type)
{
	return (type == PIPE || type == REDIR_IN || type == REDIR_OUT || type == APPEND || type == HEREDOC);
}

static t_node	*check_pipe_children(t_node *node)
{
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
	return (check_pipe_children(node));
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
