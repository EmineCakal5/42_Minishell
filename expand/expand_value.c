#include "../minishell.h"

static char	*dup_empty_string(void)
{
	char	*empty;

	empty = malloc(1);
	if (!empty)
		return (NULL);
	empty[0] = '\0';
	return (empty);
}

static int	append_plain_chunk(char **res, char *value, int *i)
{
	int	start;

	start = *i;
	while (value[*i] && value[*i] != '$' && value[*i] != '\''
		&& value[*i] != '"')
		(*i)++;
	if (*i > start && append_range_to_res(res, value, start, *i - start))
		return (1);
	return (0);
}

static int	append_expansion(char **res, char *value, int *i, char **envp)
{
	char	*name;
	char	*env_val;
	char	*num;
	int		start;

	if (value[*i + 1] == '?')
	{
		num = int_to_string(g_exit_status);
		if (!num)
			return (1);
		if (append_to_res(res, num))
		{
			free(num);
			return (1);
		}
		free(num);
		(*i) += 2;
		return (0);
	}
	if (!(value[*i + 1] == '_' || isalpha((unsigned char)value[*i + 1])))
	{
		if (append_range_to_res(res, value, *i, 1))
			return (1);
		(*i)++;
		return (0);
	}
	start = *i + 1;
	(*i)++;
	while (value[*i] && (isalnum((unsigned char)value[*i]) || value[*i] == '_'))
		(*i)++;
	name = ft_substr(value, start, *i - start);
	if (!name)
		return (1);
	env_val = env_value(envp, name);
	free(name);
	if (env_val)
	{
		if (append_to_res(res, env_val))
			return (1);
	}
	return (0);
}

static int	append_backslash(char **res, char *value, int *i, int quote)
{
	char	next;

	if (!value[*i + 1])
	{
		if (append_char_to_res(res, '\\'))
			return (1);
		(*i)++;
		return (0);
	}
	next = value[*i + 1];
	if (!quote || next == '$' || next == '"' || next == '\\')
	{
		if (append_char_to_res(res, next))
			return (1);
		(*i) += 2;
		return (0);
	}
	if (quote == '"' && next == '\n')
	{
		(*i) += 2;
		return (0);
	}
	if (append_char_to_res(res, '\\'))
		return (1);
	(*i)++;
	return (0);
}

char	*expand_value(char *value, char **envp)
{
	char	*res;
	int		i;
	int		quote;

	res = NULL;
	i = 0;
	quote = 0;
	while (value[i])
	{
		if (!quote && value[i] == '$' && value[i + 1] == '"')
		{
			i++;
			continue ;
		}
		if (quote == '\'')
		{
			if (value[i] == quote)
			{
				quote = 0;
				i++;
				continue ;
			}
			if (append_char_to_res(&res, value[i++]))
				return (free(res), NULL);
			continue ;
		}
		if (quote == '"')
		{
			if (value[i] == quote)
			{
				quote = 0;
				i++;
				continue ;
			}
			if (value[i] == '$')
			{
				if (append_expansion(&res, value, &i, envp))
					return (free(res), NULL);
				continue ;
			}
			if (value[i] == '\\')
			{
				if (append_backslash(&res, value, &i, quote))
					return (free(res), NULL);
				continue ;
			}
			if (append_char_to_res(&res, value[i++]))
				return (free(res), NULL);
			continue ;
		}
		if (value[i] == '\\' && quote != '\'')
		{
			if (append_backslash(&res, value, &i, quote))
				return (free(res), NULL);
			continue ;
		}
		if (!quote && (value[i] == '\'' || value[i] == '"'))
		{
			quote = value[i];
			i++;
			continue ;
		}
		if (quote && value[i] == quote)
		{
			quote = 0;
			i++;
			continue ;
		}
		if (value[i] == '$' && quote != '\'')
		{
			if (append_expansion(&res, value, &i, envp))
				return (free(res), NULL);
			continue ;
		}
		else
		{
			if (append_plain_chunk(&res, value, &i))
				return (free(res), NULL);
		}
	}
	if (quote)
		return (free(res), NULL);
	if (!res)
		return (dup_empty_string());
	return (res);
}
