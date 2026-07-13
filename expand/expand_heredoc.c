/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:08:02 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:03 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*strip_quotes(char *s)
{
	char	*res;
	int		i;
	char	q;

	res = NULL;
	i = 0;
	q = 0;
	while (s[i])
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
			q = s[i];
		else if (q && s[i] == q)
			q = 0;
		else if (append_char_to_res(&res, s[i]))
			return (NULL);
		i++;
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}

static int	heredoc_step(char **res, char *line, int *i, t_exp *e)
{
	if (line[*i] == '$')
		return (append_expansion(res, line, i, e));
	if (append_char_to_res(res, line[*i]))
		return (1);
	(*i)++;
	return (0);
}

char	*expand_heredoc_line(char *line, char **envp, int status)
{
	char	*res;
	int		i;
	t_exp	e;

	e.envp = envp;
	e.split = 0;
	e.status = status;
	res = NULL;
	i = 0;
	while (line[i])
	{
		if (heredoc_step(&res, line, &i, &e))
			return (free(res), NULL);
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}
