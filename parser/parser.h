/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:06:32 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:06:33 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_node	*parse(t_token *t, t_shell *sh);
int		is_op(t_type type);

t_node	*create_node_cmd(t_token *t);

void	print_ast(t_node *node, int depth);

t_redir	*new_redir(t_type type, char *target);
void	add_redir(t_redir **head, t_redir *new);
int		save_redirection(t_cmd *cmd, t_token *redir_token);

t_token	*go_end(t_token *t);
t_token	*find_right_pipe(t_token *t);

int		check_parser(t_token *t);
int		is_redirection(int type);
int		argc_counter(t_token *token);
t_cmd	*create_cmd(void);

#endif
