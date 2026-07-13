/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:27:57 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:27:58 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	unset_one(char ***envp, char *arg)
{
	int		i;
	int		j;
	char	**new_env;

	new_env = malloc(sizeof(char *) * (env_count(*envp) + 1));
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	while ((*envp)[i])
	{
		if (!same_key((*envp)[i], arg))
			new_env[j++] = (*envp)[i];
		else
			free((*envp)[i]);
		i++;
	}
	new_env[j] = NULL;
	free(*envp);
	*envp = new_env;
}

int	ft_unset(t_cmd *cmd, t_shell *sh)
{
	int	i;

	if (!sh->env)
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		unset_one(&sh->env, cmd->args[i]);
		i++;
	}
	return (0);
}
