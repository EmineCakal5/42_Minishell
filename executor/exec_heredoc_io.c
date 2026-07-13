/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_io.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:11 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:12 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"

static size_t	append_number(char *path, size_t index, unsigned long value)
{
	char	buffer[32];
	size_t	len;

	len = 0;
	if (value == 0)
		buffer[len++] = '0';
	while (value > 0)
	{
		buffer[len++] = (char)('0' + (value % 10));
		value /= 10;
	}
	while (len > 0)
	{
		len--;
		path[index++] = buffer[len];
	}
	return (index);
}

int	build_heredoc_name(char *path, size_t size)
{
	size_t					index;
	static unsigned long	counter;

	if (size < 32)
		return (-1);
	index = 0;
	while ("/tmp/heredoc_"[index])
	{
		path[index] = "/tmp/heredoc_"[index];
		index++;
	}
	counter++;
	index = append_number(path, index, counter);
	path[index] = '\0';
	return (0);
}

static char	*grow_buf(char *buf, size_t old_cap, size_t new_cap)
{
	char	*new_buf;

	new_buf = malloc(new_cap);
	if (!new_buf)
		return (free(buf), NULL);
	ft_memcpy(new_buf, buf, old_cap);
	free(buf);
	return (new_buf);
}

static int	push_char(char **buf, size_t *cap, size_t *len, char c)
{
	if (*len + 1 >= *cap)
	{
		*buf = grow_buf(*buf, *cap, *cap * 2);
		if (!*buf)
			return (1);
		*cap *= 2;
	}
	(*buf)[*len] = c;
	(*len)++;
	return (0);
}

char	*read_heredoc_line(int *cancelled)
{
	char	*buf;
	size_t	cap;
	size_t	len;
	char	c;
	int		r;

	cap = 128;
	len = 0;
	buf = malloc(cap);
	if (!buf)
		return (NULL);
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "> ", 2);
	r = read(STDIN_FILENO, &c, 1);
	while (r > 0 && c != '\n')
	{
		if (push_char(&buf, &cap, &len, c))
			return (NULL);
		r = read(STDIN_FILENO, &c, 1);
	}
	return (heredoc_finalize(buf, len, r, cancelled));
}
