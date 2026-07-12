#include "../minishell.h"

static size_t	env_count(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

void	free_env(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**copy_env(char **envp)
{
	char	**copy;
	size_t	count;
	size_t	i;

	count = env_count(envp);
	copy = ft_calloc(count + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_env(copy), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
