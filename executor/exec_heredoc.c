/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:35 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:36 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"

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
			return (free(line), 1);
		if (!line)
			return (0);
		if (str_equal(line, delimiter))
			return (free(line), 0);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	open_heredoc(char *delimiter, char *path)
{
	int					fd;
	int					cancelled;
	struct sigaction	old_int;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	reset_signals(&old_int);
	g_exit_status = 0;
	cancelled = read_heredoc_loop(fd, delimiter);
	close(fd);
	restore_signals(&old_int);
	if (cancelled)
		return (-2);
	return (0);
}
