/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:49 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:50 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stddef.h>

typedef struct s_node	t_node;
typedef struct s_cmd	t_cmd;
typedef struct s_redir	t_redir;

int		execute(t_node *node, t_shell *sh);
int		exec_cmd(t_cmd *cmd, t_shell *sh);
int		run_builtin_with_redirs(t_cmd *cmd, t_shell *sh);
int		run_redirs_only(t_cmd *cmd);
int		exec_pipe(t_node *node, t_shell *sh);
int		apply_redirections(t_redir *redir);
int		open_heredoc(t_redir *redir, char *path, t_shell *sh);
int		prepare_heredocs(t_node *node, t_shell *sh);

char	*find_cmd_path(char *cmd, char **envp);
int		command_error(char *cmd);

void	free_path_list(char **paths);
char	*join_path(char *left, char *right);
char	**split_path_dirs(char *path_line);

int		build_heredoc_name(char *path, size_t size);
char	*read_heredoc_line(int *cancelled);
char	*heredoc_finalize(char *buf, size_t len, int r, int *cancelled);

#endif
