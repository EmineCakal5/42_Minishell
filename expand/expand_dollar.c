/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:08:19 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:20 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	append_status(char **res, int *i, t_exp *e)
{
	char	*num;

	num = int_to_string(e->status);
	if (!num)
		return (1);
	if (append_to_res(res, num))
		return (free(num), 1);
	free(num);
	(*i) += 2;
	return (0);
}

static int	append_split_value(char **res, char *val)
{
	int		i;
	char	c;

	i = 0;
	while (val[i])
	{
		c = val[i];
		if (c == ' ' || c == '\t' || c == '\n')
			c = '\1';
		if (append_char_to_res(res, c))
			return (1);
		i++;
	}
	return (0);
}

static int	append_var(char **res, char *value, int *i, t_exp *e)
{
	char	*name;
	char	*env_val;
	int		start;

	start = *i + 1;
	(*i)++;
	while (value[*i] && (ft_isalnum((unsigned char)value[*i])
			|| value[*i] == '_'))
		(*i)++;
	name = ft_substr(value, start, *i - start);
	if (!name)
		return (1);
	env_val = env_value(e->envp, name);
	free(name);
	if (!env_val)
		return (0);
	if (e->split)
		return (append_split_value(res, env_val));
	return (append_to_res(res, env_val));
}

int	append_expansion(char **res, char *value, int *i, t_exp *e)
{
	if (value[*i + 1] == '?')
		return (append_status(res, i, e));
	if (!(value[*i + 1] == '_' || ft_isalpha((unsigned char)value[*i + 1])))
	{
		if (append_range_to_res(res, value, *i, 1))
			return (1);
		(*i)++;
		return (0);
	}
	return (append_var(res, value, i, e));
}
