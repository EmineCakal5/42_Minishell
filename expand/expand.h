/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:07:57 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:07:58 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

typedef struct s_exp
{
	char	**envp;
	int		split;
	int		status;
}	t_exp;

t_token	*have_expand(t_token *t, char **envp, int status);

char	*expand_value(char *value, char **envp, int status);
char	*expand_heredoc_line(char *line, char **envp, int status);
char	*strip_quotes(char *s);
char	**split_fields(char *s, int *count);
void	insert_after(t_token *node, t_token *new);
int		append_char_step(char **res, char *value, int *i);
int		append_plain_chunk(char **res, char *value, int *i);

int		in_single(char **res, char *value, int *ctx);
int		in_double(char **res, char *value, int *ctx, t_exp *e);
int		in_unquoted(char **res, char *value, int *ctx, t_exp *e);

int		append_expansion(char **res, char *value, int *i, t_exp *e);

char	*env_value(char **envp, char *name);
char	*int_to_string(int value);
char	*get_var_name(char *value, int *i);

char	*append_str_simple(char *dst, const char *src);
char	*append_substr_simple(char *dst, const char *src, int start, int len);
int		append_to_res(char **res, const char *src);
int		append_char_to_res(char **res, char c);
int		append_range_to_res(char **res, char *value, int start, int len);

#endif
