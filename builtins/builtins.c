/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:27:29 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:27:30 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	str_equal(char *left, char *right)
{
	int	i;

	i = 0;
	while (left[i] && right[i] && left[i] == right[i])
		i++;
	return (left[i] == right[i]);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (str_equal(cmd, "pwd") || str_equal(cmd, "echo")
		|| str_equal(cmd, "cd") || str_equal(cmd, "env")
		|| str_equal(cmd, "export") || str_equal(cmd, "unset")
		|| str_equal(cmd, "exit"))
		return (1);
	return (0);
}

int	run_builtin(t_cmd *cmd, char ***envp)
{
	(void)envp;
	if (str_equal(cmd->args[0], "pwd"))
		return (ft_pwd());
	if (str_equal(cmd->args[0], "env"))
		return (ft_env(*envp));
	if (str_equal(cmd->args[0], "exit"))
		return (ft_exit(cmd));
	if (str_equal(cmd->args[0], "export"))
		return (ft_export(cmd, envp));
	if (str_equal(cmd->args[0], "unset"))
		return (ft_unset(cmd, envp));
	if (str_equal(cmd->args[0], "echo"))
		return (ft_echo(cmd));
	if (str_equal(cmd->args[0], "cd"))
		return (ft_cd(cmd, envp));
	return (0);
}
