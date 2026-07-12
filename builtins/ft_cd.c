#include "../minishell.h"

static char	*shell_getenv(char **envp, char *name)
{
	int		i;
	int		j;

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

static void	update_pwd_env(char ***envp, char *new_path)
{
	int		i;
	char	*new_entry;

	i = 0;
	new_entry = ft_strjoin("PWD=", new_path);
	if (!new_entry)
		return ;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], "PWD=", 4) == 0)
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return ;
		}
		i++;
	}
	free(new_entry);
}

int	ft_cd(t_cmd *cmd, char ***envp)
{
	char	cwd[1024];
	char	*path;

	path = cmd->args[1];
	if (!path)
	{
		path = shell_getenv(*envp, "HOME");
		if (!path)
			return (0);
	}
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		update_pwd_env(envp, cwd);
	return (0);
}
