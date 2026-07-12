#ifndef EXPAND_H
# define EXPAND_H

void	have_expand(t_token *t, char **envp);

char	*expand_value(char *value, char **envp);
int		append_char_step(char **res, char *value, int *i);
int		append_plain_chunk(char **res, char *value, int *i);

int		in_single(char **res, char *value, int *ctx);
int		in_double(char **res, char *value, int *ctx, char **envp);
int		in_unquoted(char **res, char *value, int *ctx, char **envp);

int		append_expansion(char **res, char *value, int *i, char **envp);

char	*env_value(char **envp, char *name);
char	*int_to_string(int value);
char	*get_var_name(char *value, int *i);

char	*append_str_simple(char *dst, const char *src);
char	*append_substr_simple(char *dst, const char *src, int start, int len);
int		append_to_res(char **res, const char *src);
int		append_char_to_res(char **res, char c);
int		append_range_to_res(char **res, char *value, int start, int len);

#endif
