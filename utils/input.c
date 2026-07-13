/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:29:31 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:29:32 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*append_char(char *s, char c)
{
	char	ch[2];

	ch[0] = c;
	ch[1] = '\0';
	return (ft_strjoin(s, ch));
}

char	*read_input_line(void)
{
	char	*line;
	char	*tmp;
	char	c;
	int		r;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	r = read(STDIN_FILENO, &c, 1);
	while (r > 0)
	{
		if (c == '\n')
			return (line);
		tmp = append_char(line, c);
		free(line);
		if (!tmp)
			return (NULL);
		line = tmp;
		r = read(STDIN_FILENO, &c, 1);
	}
	if (r <= 0 && line[0] == '\0')
		return (free(line), NULL);
	return (line);
}
