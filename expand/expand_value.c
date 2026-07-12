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

int	append_char_step(char **res, char *value, int *i)
{
	if (append_char_to_res(res, value[*i]))
		return (1);
	(*i)++;
	return (0);
}

int	append_plain_chunk(char **res, char *value, int *i)
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

static int	dispatch(char **res, char *value, int *ctx, char **envp)
{
	if (!ctx[1] && value[ctx[0]] == '$' && value[ctx[0] + 1] == '"')
	{
		ctx[0]++;
		return (0);
	}
	if (ctx[1] == '\'')
		return (in_single(res, value, ctx));
	if (ctx[1] == '"')
		return (in_double(res, value, ctx, envp));
	return (in_unquoted(res, value, ctx, envp));
}

char	*expand_value(char *value, char **envp)
{
	char	*res;
	int		ctx[2];

	res = NULL;
	ctx[0] = 0;
	ctx[1] = 0;
	while (value[ctx[0]])
	{
		if (dispatch(&res, value, ctx, envp))
			return (free(res), NULL);
	}
	if (ctx[1])
		return (free(res), NULL);
	if (!res)
		return (dup_empty_string());
	return (res);
}
