#include "../minishell.h"

static int	key_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	return (i);
}

static int	key_match(char *env, char *arg)
{
	int	i;
	int	len;

	i = 0;
	len = key_len(arg);
	while (i < len && env[i] && env[i] == arg[i])
		i++;
	return (i == len && env[i] == '=');
}

static int	env_count(char **envp)
{
	int	count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
}

static char	*shell_strdup(char *s)
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

static char	*build_entry(char *arg)
{
	int		len;
	int		has_eq;
	char	*entry;
	int		i;

	len = key_len(arg);
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

static int	add_new_entry(char ***envp, char *new_entry)
{
	char	**new_env;
	int		i;
	int		count;

	count = env_count(*envp);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_entry);
		return (1);
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

static int	export_one(char ***envp, char *arg)
{
	char	*new_entry;
	int		i;

	new_entry = build_entry(arg);
	if (!new_entry)
		return (1);
	i = 0;
	while ((*envp)[i])
	{
		if (key_match((*envp)[i], arg))
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return (0);
		}
		i++;
	}
	return (add_new_entry(envp, new_entry));
}

int ft_export(t_cmd *cmd, char ***envp)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	if (!envp || !*envp)
		return (0);
	if (!cmd->args[1])
	{
		ft_env(*envp);
		return (0);
	}
	while (cmd->args[i])
	{
		if (export_one(envp, cmd->args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}