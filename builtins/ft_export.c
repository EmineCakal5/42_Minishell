/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:27:49 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:27:50 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	add_new_entry(char ***envp, char *new_entry)
{
	char	**new_env;
	int		i;
	int		count;

	count = export_env_count(*envp);
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

int	ft_export(t_cmd *cmd, char ***envp)
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
