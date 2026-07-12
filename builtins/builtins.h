/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:27:33 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:27:34 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int		export_key_len(char *arg);
int		key_match(char *env, char *arg);
int		export_env_count(char **envp);
char	*shell_strdup(char *s);
char	*build_entry(char *arg);

#endif
