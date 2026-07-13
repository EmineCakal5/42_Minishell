/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:59 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:01 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*env_value(char **envp, char *name)
{
	int	i;
	int	j;

	if (!envp || !name)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && name[j] && envp[i][j] == name[j])
			j++;
		if (name[j] == '\0' && envp[i][j] == '=')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

static int	num_len(long nbr, int sign)
{
	int	len;

	if (nbr == 0)
		return (1 + sign);
	len = sign;
	while (nbr > 0)
	{
		len++;
		nbr /= 10;
	}
	return (len);
}

char	*int_to_string(int value)
{
	char	*res;
	long	nbr;
	int		len;
	int		sign;

	nbr = value;
	sign = (value < 0);
	if (nbr < 0)
		nbr = -nbr;
	len = num_len(nbr, sign);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (sign)
		res[0] = '-';
	if (nbr == 0)
		res[sign] = '0';
	while (nbr > 0)
	{
		res[--len] = '0' + (nbr % 10);
		nbr /= 10;
	}
	return (res);
}

char	*get_var_name(char *value, int *i)
{
	int	start;

	(*i)++;
	start = *i;
	while (value[*i] && (ft_isalnum((unsigned char)value[*i])
			|| value[*i] == '_'))
		(*i)++;
	return (ft_substr(value, start, *i - start));
}
