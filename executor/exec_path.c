/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:37 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:38 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"
#include <sys/stat.h>

static int	is_executable_file(char *path)
{
	struct stat	st;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	return (!S_ISDIR(st.st_mode));
}

static char	**get_path_dirs(char **envp)
{
	int		i;
	char	*path_line;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			path_line = envp[i] + 5;
			return (split_path_dirs(path_line));
		}
		i++;
	}
	return (NULL);
}

static char	*join_cmd_path(char *dir, char *cmd)
{
	char	*full_path;

	full_path = join_path(dir, cmd);
	return (full_path);
}

static char	*find_cmd_in_path(char **paths, char *cmd)
{
	int		i;
	char	*full_path;

	if (!paths || !cmd)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_cmd_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (is_executable_file(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (is_executable_file(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = get_path_dirs(envp);
	if (!paths)
		return (NULL);
	cmd_path = find_cmd_in_path(paths, cmd);
	free_path_list(paths);
	return (cmd_path);
}
