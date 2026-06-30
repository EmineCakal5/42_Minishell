#ifndef EXPAND_H
# define EXPAND_H

/* expand/expand.c */
void	have_expand(t_token *t);

/* expand/expand_utils.c */
char	*expand_value(char *value);
char	*get_var_name(char *value, int *i);
char	*append_str_simple(char *dst, const char *src);
char	*append_substr_simple(char *dst, const char *src, int start, int len);

#endif
