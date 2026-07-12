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

	left_len = ft_strlen(left);
	right_len = ft_strlen(right);
	full_path = malloc(left_len + right_len + 2);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, left, left_len);
	full_path[left_len] = '/';
	ft_memcpy(full_path + left_len + 1, right, right_len);
	full_path[left_len + right_len + 1] = '\0';
	return (full_path);
}

static int	count_colons(char *s)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == ':')
			count++;
		i++;
	}
	return (count);
}

static char	*dup_range(char *s, int start, int end)
{
	char	*out;
	int		i;

	out = malloc(end - start + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		out[i] = s[start + i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

char	**split_path_dirs(char *path_line)
{
	char	**paths;
	int		i;
	int		start;
	int		index;

	paths = ft_calloc(count_colons(path_line) + 1, sizeof(char *));
	if (!paths)
		return (NULL);
	start = 0;
	index = 0;
	i = 0;
	while (1)
	{
		if (path_line[i] == ':' || path_line[i] == '\0')
		{
			paths[index] = dup_range(path_line, start, i);
			if (!paths[index++])
				return (free_path_list(paths), NULL);
			if (path_line[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	return (paths);
}
