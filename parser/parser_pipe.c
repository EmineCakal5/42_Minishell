#include "../minishell.h"

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
