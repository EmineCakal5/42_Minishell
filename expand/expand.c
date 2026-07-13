/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:54 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:07:55 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	should_expand(t_token *t)
{
	if (!t->value || t->type != WORD)
		return (0);
	if (t->prev && t->prev->type == HEREDOC)
		return (0);
	return (1);
}

static t_token	*apply_fields(t_token *t, char **fields, int n)
{
	int		k;
	t_token	*node;
	t_token	*ins;

	free(t->value);
	t->value = fields[0];
	node = t;
	k = 1;
	while (k < n)
	{
		ins = new_token(WORD, fields[k]);
		insert_after(node, ins);
		node = ins;
		k++;
	}
	return (node->next);
}

static t_token	*drop_empty(t_token *t, t_token **head)
{
	t_token	*next;

	if (t->quoted)
	{
		free(t->value);
		t->value = ft_strdup("");
		return (t->next);
	}
	next = t->next;
	if (t->prev)
		t->prev->next = next;
	else
		*head = next;
	if (next)
		next->prev = t->prev;
	free(t->value);
	free(t);
	return (next);
}

static t_token	*expand_one(t_token *t, char **envp, int status, t_token **head)
{
	char	*val;
	char	**fields;
	int		n;
	t_token	*next;

	val = expand_value(t->value, envp, status);
	if (!val)
		return (t->next);
	fields = split_fields(val, &n);
	free(val);
	if (!fields)
		return (t->next);
	if (n > 0)
	{
		next = apply_fields(t, fields, n);
		free(fields);
		return (next);
	}
	free(fields);
	return (drop_empty(t, head));
}

t_token	*have_expand(t_token *t, char **envp, int status)
{
	t_token	*head;
	t_token	*cur;

	head = t;
	cur = t;
	while (cur)
	{
		if (should_expand(cur))
			cur = expand_one(cur, envp, status, &head);
		else
			cur = cur->next;
	}
	return (head);
}
