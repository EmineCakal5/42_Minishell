#include "../minishell.h"

static int	syntax_error(void)
{
	write(2, "minishell: syntax error\n", 24);
	return (1);
}

int	pipe_control(char *str, int i)
{
	int	j;

	if (str[i] != '|')
		return (0);
	if (i == 0)
		return (syntax_error());
	j = i - 1;
	while (j >= 0 && is_empty(str[j]))
		j--;
	if (j < 0)
		return (syntax_error());
	j = i + 1;
	while (str[j] && is_empty(str[j]))
		j++;
	if (!str[j])
		return (syntax_error());
	return (0);
}

int	double_token(char *str, int i)
{
	if (op(str[i]) && op(str[i + 1]))
	{
		if (!((str[i] == '<' && str[i + 1] == '<') ||
				(str[i] == '>' && str[i + 1] == '>')))
		{
			return (syntax_error());
		}
	}
	return (0);
}

int	after_token(char *s, int i)
{
	if (op(s[i]))
	{
		i += op_number(s, i);
		while (s[i] && is_empty(s[i]))
			i++;
		if (!s[i] || op(s[i]))
			return (syntax_error());
		return (0);
	}
	return (0);
}

char	*many_word_control(char *s, int *i, char c)
{
	int		start;
	char	*new;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	if (s[*i] != c)
		return (NULL);
	new = ft_substr(s, start, *i - start);
	(*i)++;
	return (new);
}
