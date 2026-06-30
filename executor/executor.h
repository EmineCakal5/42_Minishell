#ifndef EXECUTOR_H
# define EXECUTOR_H

// Forward declarations (Bu struct'ların var olduğunu derleyiciye söyler)
typedef struct s_node t_node;
typedef struct s_cmd t_cmd;
typedef struct s_redir t_redir;

int	execute(t_node *node, char **envp);
int		exec_cmd(t_cmd *cmd, char **envp);
void exec_pipe(t_node *node, char **envp);
int apply_redirections(t_redir *redir);
int open_heredoc(char *delimiter);

#endif
