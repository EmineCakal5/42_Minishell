/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecakal <ecakal@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 20:28:09 by ecakal            #+#    #+#             */
/*   Updated: 2026/07/12 20:28:09 by ecakal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"
#include <sys/stat.h>

static int	cmd_err(char *cmd, char *msg, int code)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (code);
}

int	command_error(char *cmd)
{
	struct stat	st;

	if (!ft_strchr(cmd, '/'))
		return (cmd_err(cmd, "command not found", 127));
	if (stat(cmd, &st) != 0)
		return (cmd_err(cmd, "No such file or directory", 127));
	if (S_ISDIR(st.st_mode))
		return (cmd_err(cmd, "Is a directory", 126));
	return (cmd_err(cmd, "Permission denied", 126));
}
