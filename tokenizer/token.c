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

static int	word_quote_state(char *value)
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
	t_token	*tok;
	char	*new;
	int		i;
	int		len;
	t_type	type;

	head = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_empty(s[i]))
			i++;
		if (!s[i])
			break ;
		if (pipe_control(s, i) || double_token(s, i) || after_token(s, i))
		{
			free_tokens(head);
			return (NULL);
		}
		if (op(s[i]))
		{
			len = op_number(s, i);
			type = op_type(s, i);
			new = ft_substr(s, i, len);
			if (!new)
			{
				free_tokens(head);
				return (NULL);
			}
			tok = new_token(type, new);
			if (!tok)
			{
				free(new);
				free_tokens(head);
				return (NULL);
			}
			add_token(&head, tok);
			i += len;
		}
		else
		{
			new = read_word(s, &i);
			if (!new)
			{
				free_tokens(head);
				return (NULL);
			}
			tok = new_token(WORD, new);
			if (!tok)
			{
				free(new);
				free_tokens(head);
				return (NULL);
			}
			tok->quoted = word_quote_state(new);
			add_token(&head, tok);
		}
	}
	return (head);
}