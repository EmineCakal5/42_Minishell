#include "../minishell.h"

t_token	*new_token(t_type type, char *value)
{
	t_token	*t;

	t = (t_token *)malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->type = type;
	t->value = value;
	t->quoted = 0;
	t->next = NULL;
	t->prev = NULL;
	return (t);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
	new->next = NULL;
}

int	word_quote_state(char *value)
{
	int	i;
	int	quote;
	int	seen_quote;

	i = 0;
	quote = 0;
	seen_quote = 0;
	while (value[i])
	{
		if ((value[i] == '\'' || value[i] == '"') && quote == 0)
		{
			quote = value[i];
			seen_quote = 1;
		}
		else if (value[i] == quote)
			quote = 0;
		i++;
	}
	return (seen_quote);
}

char	*read_word(char *s, int *i)
{
	int	start;
	int	quote;

	start = *i;
	quote = 0;
	while (s[*i])
	{
		if (!quote && is_empty(s[*i]))
			break ;
		if (!quote && op(s[*i]))
			break ;
		if (s[*i] == '\'' || s[*i] == '"')
		{
			if (!quote)
				quote = s[*i];
			else if (quote == s[*i])
				quote = 0;
		}
		(*i)++;
	}
	if (quote)
		return (NULL);
	return (ft_substr(s, start, *i - start));
}

t_token	*tokenize(char *s)
{
	t_token	*head;
	int		i;
	int		fail;

	head = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_empty(s[i]))
			i++;
		if (!s[i])
			break ;
		if (pipe_control(s, i) || double_token(s, i) || after_token(s, i))
			return (free_tokens(head), NULL);
		if (op(s[i]))
			fail = handle_op_token(s, &i, &head);
		else
			fail = handle_word_token(s, &i, &head);
		if (fail)
			return (free_tokens(head), NULL);
	}
	return (head);
}
