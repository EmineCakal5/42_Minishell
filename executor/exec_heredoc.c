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

char	*heredoc_finalize(char *buf, size_t len, int r, int *cancelled)
{
	*cancelled = (r < 0);
	if (r < 0)
		return (free(buf), NULL);
	if (r == 0 && len == 0)
		return (free(buf), NULL);
	buf[len] = '\0';
	return (buf);
}

static void	write_heredoc_line(int fd, char *line, t_redir *redir, t_shell *sh)
{
	char	*out;

	out = line;
	if (redir->expand)
		out = expand_heredoc_line(line, sh->env, sh->status);
	if (!out)
		return ;
	write(fd, out, ft_strlen(out));
	write(fd, "\n", 1);
	if (out != line)
		free(out);
}

static int	read_heredoc_loop(int fd, t_redir *redir, t_shell *sh)
{
	char	*line;
	int		cancelled;

	while (1)
	{
		line = read_heredoc_line(&cancelled);
		if (cancelled)
			return (free(line), 1);
		if (!line)
			return (0);
		if (str_equal(line, redir->target))
			return (free(line), 0);
		write_heredoc_line(fd, line, redir, sh);
		free(line);
	}
}

int	open_heredoc(t_redir *redir, char *path, t_shell *sh)
{
	int					fd;
	int					cancelled;
	struct sigaction	old_int;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	reset_signals(&old_int);
	cancelled = read_heredoc_loop(fd, redir, sh);
	close(fd);
	restore_signals(&old_int);
	if (cancelled)
		return (-2);
	return (0);
}
