#include "../minishell.h"

static int	str_equal(const char *left, const char *right)
{
	while (*left && *right && *left == *right)
	{
		left++;
		right++;
	}
	return (*left == *right);
}

static int	read_heredoc_loop(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (g_exit_status == 130)
		{
			free(line);
			return (1);
		}
		if (!line)
			return (0);
		if (str_equal(line, delimiter))
		{
			free(line);
			return (0);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	open_heredoc(char *delimiter)
{
	int					fd;
	int					cancelled;
	char				path[64];
	struct sigaction	old_int;

	if (build_heredoc_name(path, sizeof(path)) == -1)
		return (-1);
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	reset_signals(&old_int);
	g_exit_status = 0;
	cancelled = read_heredoc_loop(fd, delimiter);
	close(fd);
	if (cancelled)
	{
		unlink(path);
		restore_signals(&old_int);
		return (-2);
	}
	fd = open(path, O_RDONLY);
	unlink(path);
	restore_signals(&old_int);
	return (fd);
}
