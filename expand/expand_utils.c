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

static char	*env_value(char **envp, char *name)
{
	int	i;
	int	j;

	if (!envp || !name)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && name[j] && envp[i][j] == name[j])
			j++;
		if (name[j] == '\0' && envp[i][j] == '=')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

static char	*int_to_string(int value)
{
	char	buffer[12];
	int		len;
	long	nbr;
	int		sign;
	char	*res;
	int		i;

	nbr = value;
	sign = 0;
	len = 0;
	if (nbr < 0)
	{
		sign = 1;
		nbr = -nbr;
	}
	if (nbr == 0)
		buffer[len++] = '0';
	while (nbr > 0)
	{
		buffer[len++] = (char)('0' + (nbr % 10));
		nbr /= 10;
	}
	res = malloc(len + sign + 1);
	if (!res)
		return (NULL);
	res[len + sign] = '\0';
	if (sign)
		res[0] = '-';
	i = 0;
	while (i < len)
	{
		res[i + sign] = buffer[len - 1 - i];
		i++;
	}
	return (res);
}

char	*append_str_simple(char *dst, const char *src)
{
	size_t	old_len;
	size_t	src_len;
	char	*tmp;

	old_len = dst ? strlen(dst) : 0;
	src_len = src ? strlen(src) : 0;
	tmp = realloc(dst, old_len + src_len + 1);
	if (!tmp)
		return (NULL);
	if (src_len > 0)
		memcpy(tmp + old_len, src, src_len);
	tmp[old_len + src_len] = '\0';
	return (tmp);
}

char	*append_substr_simple(char *dst, const char *src, int start, int len)
{
	size_t	old_len;
	char	*tmp;

	old_len = dst ? strlen(dst) : 0;
	tmp = realloc(dst, old_len + len + 1);
	if (!tmp)
		return (NULL);
	if (len > 0)
		memcpy(tmp + old_len, src + start, len);
	tmp[old_len + len] = '\0';
	return (tmp);
}

char	*get_var_name(char *value, int *i)
{
	int	start;

	(*i)++;
	start = *i;
	while (value[*i] && (isalnum((unsigned char)value[*i]) || value[*i] == '_'))
		(*i)++;
	return (ft_substr(value, start, *i - start));
}

static int	append_to_res(char **res, const char *src)
{
	char	*tmp;

	tmp = append_str_simple(*res, src);
	if (!tmp)
		return (1);
	*res = tmp;
	return (0);
}

static int	append_char_to_res(char **res, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_to_res(res, buf));
}

static int	append_range_to_res(char **res, char *value, int start, int len)
{
	char	*tmp;

	tmp = append_substr_simple(*res, value, start, len);
	if (!tmp)
		return (1);
	*res = tmp;
	return (0);
}

static int	append_plain_chunk(char **res, char *value, int *i, char stop_a, char stop_b)
{
	int	start;

	start = *i;
	while (value[*i] && value[*i] != '$' && value[*i] != stop_a && value[*i] != stop_b)
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

	/* $? shell'in son durum kodunu verir. */
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
			if (append_plain_chunk(&res, value, &i, '\'', '"'))
				return (free(res), NULL);
		}
	}
	if (quote)
		return (free(res), NULL);
	if (!res)
		return (dup_empty_string());
	return (res);
}
