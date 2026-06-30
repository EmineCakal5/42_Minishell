#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

// Forward declarations (Yapıların var olduğunu derleyiciye bildiriyoruz)
typedef struct s_token t_token;
typedef struct s_node t_node;
typedef struct s_cmd t_cmd;
typedef struct s_redir t_redir;
/* utils/utils.c */
char	*ft_substr(const char *s, unsigned int start, size_t len);
int ft_atoi(const char *str);


/* utils/free.c */
void	free_tokens(t_token *head);
void	free_ast(t_node *node);
void	free_cmd_(t_cmd *cmd);
void	free_redirs(t_redir *redir);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
#endif
