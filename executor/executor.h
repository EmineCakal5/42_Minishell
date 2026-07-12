#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stddef.h>

typedef struct s_node	t_node;
typedef struct s_cmd	t_cmd;
typedef struct s_redir	t_redir;

int		execute(t_node *node, char ***envp);
int		exec_cmd(t_cmd *cmd, char ***envp);
int		run_builtin_with_redirs(t_cmd *cmd, char ***envp);
void	exec_pipe(t_node *node, char ***envp);
int		apply_redirections(t_redir *redir);
int		open_heredoc(char *delimiter, char *path);
int		prepare_heredocs(t_node *node);

char	*find_cmd_path(char *cmd, char **envp);

void	free_path_list(char **paths);
char	*join_path(char *left, char *right);
char	**split_path_dirs(char *path_line);

int		build_heredoc_name(char *path, size_t size);
char	*read_heredoc_line(void);

#endif
