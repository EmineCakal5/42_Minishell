#include "../minishell.h"

static int	append_status(char **res, int *i)
{
	char	*num;

	num = int_to_string(g_exit_status);
	if (!num)
		return (1);
	if (append_to_res(res, num))
		return (free(num), 1);
	free(num);
	(*i) += 2;
	return (0);
}

static int	append_var(char **res, char *value, int *i, char **envp)
{
	char	*name;
	char	*env_val;
	int		start;

	start = *i + 1;
	(*i)++;
	while (value[*i] && (ft_isalnum((unsigned char)value[*i])
			|| value[*i] == '_'))
		(*i)++;
	name = ft_substr(value, start, *i - start);
	if (!name)
		return (1);
	env_val = env_value(envp, name);
	free(name);
	if (env_val && append_to_res(res, env_val))
		return (1);
	return (0);
}

int	append_expansion(char **res, char *value, int *i, char **envp)
{
	if (value[*i + 1] == '?')
		return (append_status(res, i));
	if (!(value[*i + 1] == '_' || ft_isalpha((unsigned char)value[*i + 1])))
	{
		if (append_range_to_res(res, value, *i, 1))
			return (1);
		(*i)++;
		return (0);
	}
	return (append_var(res, value, i, envp));
}
