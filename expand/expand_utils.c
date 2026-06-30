#include "../minishell.h"

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

static int	append_expansion(char **res, char *value, int *i)
{
	char	*name;
	char	*env_val;
	char	buffer[16];
	int		start;

	/* $? shell'in son durum kodunu verir. */
	if (value[*i + 1] == '?')
	{
		snprintf(buffer, sizeof(buffer), "%d", g_exit_status);
		if (append_to_res(res, buffer))
			return (1);
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
	env_val = getenv(name);
	free(name);
	if (env_val)
	{
		if (append_to_res(res, env_val))
			return (1);
	}
	return (0);
}

char	*expand_value(char *value)
{
	char	*res;
	int		i;
	int		quote;

	res = NULL;
	i = 0;
	quote = 0;
	while (value[i])
	{
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
			if (append_expansion(&res, value, &i))
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
	return (res);
}
