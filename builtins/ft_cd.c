/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:27:35 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:27:36 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	set_env_kv(char ***envp, char *prefix, char *val)
{
	int		i;
	int		plen;
	char	*new_entry;

	new_entry = ft_strjoin(prefix, val);
	if (!new_entry)
		return ;
	plen = ft_strlen(prefix);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], prefix, plen) == 0)
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return ;
		}
		i++;
	}
	free(new_entry);
}

static char	*cd_target(t_cmd *cmd, char **envp, int *print)
{
	*print = 0;
	if (!cmd->args[1])
		return (shell_getenv(envp, "HOME"));
	if (!ft_strcmp(cmd->args[1], "-"))
	{
		*print = 1;
		return (shell_getenv(envp, "OLDPWD"));
	}
	return (cmd->args[1]);
}

int	ft_cd(t_cmd *cmd, t_shell *sh)
{
	char	cwd[1024];
	char	old[1024];
	char	*path;
	int		print;

	if (cmd->args[1] && cmd->args[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	if (!getcwd(old, sizeof(old)))
		old[0] = '\0';
	path = cd_target(cmd, sh->env, &print);
	if (!path)
		return (0);
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	if (print)
		(write(1, path, ft_strlen(path)), write(1, "\n", 1));
	set_env_kv(&sh->env, "OLDPWD=", old);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_kv(&sh->env, "PWD=", cwd);
	return (0);
}
