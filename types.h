/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:08:48 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:49 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_type;

typedef enum e_node_type
{
	LEAD_CMD,
	LEAF_PIPE
}	t_node_type;

typedef struct s_shell
{
	char	**env;
	int		status;
}	t_shell;

typedef struct s_redir
{
	t_type			type;
	char			*target;
	char			*hd_path;
	int				expand;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				heredoc_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	t_type			type;
	char			*value;
	int				quoted;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_cmd			*cmd;
}	t_node;

#endif
