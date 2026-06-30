#ifndef BUILTINS_H
# define BUILTINS_H


int		is_builtin(char *cmd);
int		run_builtin(t_cmd *cmd, char ***envp);

int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd, char ***envp);
int		ft_pwd(void);
int		ft_export(t_cmd *cmd, char ***envp);
int		ft_unset(t_cmd *cmd, char ***envp);
int		ft_env(char **envp);
int		ft_exit(t_cmd *cmd);

#endif
