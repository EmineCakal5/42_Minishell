#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

// Forward declarations (Yapıların var olduğunu derleyiciye bildiriyoruz)
typedef struct s_token	t_token;
typedef struct s_node	t_node;
typedef struct s_cmd	t_cmd;
typedef struct s_redir	t_redir;
/* utils/utils.c */
char	*ft_substr(const char *s, unsigned int start, size_t len);
int		ft_atoi(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);

/* utils/ft_string.c */
char	*ft_strdup(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strchr(const char *s, int c);

/* utils/ft_mem.c */
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

/* utils/ft_char.c */
int		ft_isalpha(int c);
int		ft_isalnum(int c);

/* utils/free.c */
void	free_tokens(t_token *head);
void	free_ast(t_node *node);
void	free_cmd_(t_cmd *cmd);
void	free_redirs(t_redir *redir);

/* utils/env_utils.c */
char	**copy_env(char **envp);
void	free_env(char **envp);
#endif
