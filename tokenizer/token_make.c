/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_make.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:44 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:07:45 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_op_token(char *s, int *i, t_token **head)
{
	t_token	*tok;
	char	*new;
	int		len;
	t_type	type;

	len = op_number(s, *i);
	type = op_type(s, *i);
	new = ft_substr(s, *i, len);
	if (!new)
		return (1);
	tok = new_token(type, new);
	if (!tok)
	{
		free(new);
		return (1);
	}
	add_token(head, tok);
	*i += len;
	return (0);
}

int	handle_word_token(char *s, int *i, t_token **head)
{
	t_token	*tok;
	char	*new;

	new = read_word(s, i);
	if (!new)
		return (1);
	tok = new_token(WORD, new);
	if (!tok)
	{
		free(new);
		return (1);
	}
	tok->quoted = word_quote_state(new);
	add_token(head, tok);
	return (0);
}
