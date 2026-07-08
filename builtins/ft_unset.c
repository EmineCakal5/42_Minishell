#include "../minishell.h"

static int	key_len(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	return (i);
}

static int	same_key(char *env, char *arg)
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

/*unset komutu, envp dizisini tarar,
 silinecek anahtarı bulur ve 
o elemanı diziden çıkartıp diziyi tekrar birleştirir.*/

int	ft_unset(t_cmd *cmd, char ***envp)
{
	int		i;
	int		j;
	char	**new_env;

	if (!cmd->args[1])
		return (0);
	if (!envp || !*envp)
		return (0);

	new_env = malloc(sizeof(char *) * (env_count(*envp) + 1));
	if (!new_env)
		return (1);

	i = 0;
	j = 0;
	while ((*envp)[i])
	{
		if (!same_key((*envp)[i], cmd->args[1]))
			new_env[j++] = (*envp)[i];
		else
			free((*envp)[i]);
		i++;
	}
	new_env[j] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}