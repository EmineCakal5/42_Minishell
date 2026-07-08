#include "../minishell.h"
#include "executor.h"

void	free_path_list(char **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*join_path(char *left, char *right)
{
	char	*full_path;
	size_t	left_len;
	size_t	right_len;

	left_len = strlen(left);
	right_len = strlen(right);
	full_path = malloc(left_len + right_len + 2);
	if (!full_path)
		return (NULL);
	memcpy(full_path, left, left_len);
	full_path[left_len] = '/';
	memcpy(full_path + left_len + 1, right, right_len);
	full_path[left_len + right_len + 1] = '\0';
	return (full_path);
}

char	**split_path_dirs(char *path_line)
{
	char	**paths;
	int		count;
	int		i;
	int		start;
	int		index;
	int		len;

	count = 1;
	i = 0;
	while (path_line[i])
	{
		if (path_line[i] == ':')
			count++;
		i++;
	}
	paths = calloc(count + 1, sizeof(char *));
	if (!paths)
		return (NULL);
	start = 0;
	index = 0;
	i = 0;
	while (1)
	{
		if (path_line[i] == ':' || path_line[i] == '\0')
		{
			len = i - start;
			paths[index] = malloc(len + 1);
			if (!paths[index])
			{
				free_path_list(paths);
				return (NULL);
			}
			memcpy(paths[index], path_line + start, len);
			paths[index][len] = '\0';
			index++;
			if (path_line[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	return (paths);
}
