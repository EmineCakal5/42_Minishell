#include "../minishell.h"

int	export_key_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	return (i);
}

int	key_match(char *env, char *arg)
{
	int	i;
	int	len;

	i = 0;
	len = export_key_len(arg);
	while (i < len && env[i] && env[i] == arg[i])
		i++;
	return (i == len && env[i] == '=');
}

int	export_env_count(char **envp)
{
	int	count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
}

char	*shell_strdup(char *s)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i])
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*build_entry(char *arg)
{
	int		len;
	int		has_eq;
	char	*entry;
	int		i;

	len = export_key_len(arg);
	has_eq = (arg[len] == '=');
	if (has_eq)
		return (shell_strdup(arg));
	entry = malloc(len + 2);
	if (!entry)
		return (NULL);
	i = 0;
	while (i < len)
	{
		entry[i] = arg[i];
		i++;
	}
	entry[len] = '=';
	entry[len + 1] = '\0';
	return (entry);
}
