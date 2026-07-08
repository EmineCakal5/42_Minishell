#include "../minishell.h"

char	*env_value(char **envp, char *name)
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

char	*int_to_string(int value)
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

char	*get_var_name(char *value, int *i)
{
	int	start;

	(*i)++;
	start = *i;
	while (value[*i] && (isalnum((unsigned char)value[*i]) || value[*i] == '_'))
		(*i)++;
	return (ft_substr(value, start, *i - start));
}
