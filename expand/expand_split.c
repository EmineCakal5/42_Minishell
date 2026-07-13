/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:08:06 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:07 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_after(t_token *node, t_token *new)
{
	new->next = node->next;
	new->prev = node;
	if (node->next)
		node->next->prev = new;
	node->next = new;
}

static int	count_fields(char *s)
{
	int	i;
	int	n;

	n = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == '\1')
			i++;
		if (s[i])
		{
			n++;
			while (s[i] && s[i] != '\1')
				i++;
		}
	}
	return (n);
}

static char	*field_dup(char *s, int *i)
{
	int	start;

	while (s[*i] == '\1')
		(*i)++;
	start = *i;
	while (s[*i] && s[*i] != '\1')
		(*i)++;
	return (ft_substr(s, start, *i - start));
}

char	**split_fields(char *s, int *count)
{
	char	**arr;
	int		n;
	int		i;
	int		k;

	n = count_fields(s);
	*count = n;
	arr = malloc(sizeof(char *) * (n + 1));
	if (!arr)
		return (NULL);
	i = 0;
	k = 0;
	while (k < n)
	{
		arr[k] = field_dup(s, &i);
		if (!arr[k])
			return (arr[k] = NULL, arr);
		k++;
	}
	arr[n] = NULL;
	return (arr);
}
