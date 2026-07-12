#include "../minishell.h"

static int	put_and_step(char **res, char c, int *i, int step)
{
	if (append_char_to_res(res, c))
		return (1);
	(*i) += step;
	return (0);
}

static int	append_backslash(char **res, char *value, int *i, int quote)
{
	char	next;

	if (!value[*i + 1])
		return (put_and_step(res, '\\', i, 1));
	next = value[*i + 1];
	if (!quote || next == '$' || next == '"' || next == '\\')
		return (put_and_step(res, next, i, 2));
	if (quote == '"' && next == '\n')
	{
		(*i) += 2;
		return (0);
	}
	return (put_and_step(res, '\\', i, 1));
}

int	in_single(char **res, char *value, int *ctx)
{
	if (value[ctx[0]] == '\'')
	{
		ctx[1] = 0;
		ctx[0]++;
		return (0);
	}
	return (append_char_step(res, value, &ctx[0]));
}

int	in_double(char **res, char *value, int *ctx, char **envp)
{
	if (value[ctx[0]] == '"')
	{
		ctx[1] = 0;
		ctx[0]++;
		return (0);
	}
	if (value[ctx[0]] == '$')
		return (append_expansion(res, value, &ctx[0], envp));
	if (value[ctx[0]] == '\\')
		return (append_backslash(res, value, &ctx[0], '"'));
	return (append_char_step(res, value, &ctx[0]));
}

int	in_unquoted(char **res, char *value, int *ctx, char **envp)
{
	if (value[ctx[0]] == '\\')
		return (append_backslash(res, value, &ctx[0], 0));
	if (value[ctx[0]] == '\'' || value[ctx[0]] == '"')
	{
		ctx[1] = value[ctx[0]];
		ctx[0]++;
		return (0);
	}
	if (value[ctx[0]] == '$')
		return (append_expansion(res, value, &ctx[0], envp));
	return (append_plain_chunk(res, value, &ctx[0]));
}
