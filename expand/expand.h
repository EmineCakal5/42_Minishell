#ifndef EXPAND_H
# define EXPAND_H

/* expand/expand.c */
void	have_expand(t_token *t, char **envp);

/* expand/expand_value.c */
char	*expand_value(char *value, char **envp);

/* expand/expand_env.c */
char	*env_value(char **envp, char *name);
char	*int_to_string(int value);
char	*get_var_name(char *value, int *i);

/* expand/expand_append.c */
char	*append_str_simple(char *dst, const char *src);
char	*append_substr_simple(char *dst, const char *src, int start, int len);
int		append_to_res(char **res, const char *src);
int		append_char_to_res(char **res, char c);
int		append_range_to_res(char **res, char *value, int start, int len);

#endif
