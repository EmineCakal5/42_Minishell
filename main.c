/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkutlu <zkutlu@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:08:44 by zkutlu            #+#    #+#             */
/*   Updated: 2026/07/13 04:08:45 by zkutlu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(char *line, t_shell *sh)
{
	t_token	*tokens;
	t_node	*ast;

	if (*line)
		add_history(line);
	tokens = tokenize(line, sh);
	if (!tokens)
		return ;
	tokens = have_expand(tokens, sh->env, sh->status);
	ast = parse(tokens, sh);
	if (!ast)
		return ;
	if (prepare_heredocs(ast, sh) == -1)
	{
		free_ast(ast);
		return ;
	}
	sh->status = execute(ast, sh);
	free_ast(ast);
}

static char	*read_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline("minishell> "));
	return (read_input_line());
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	sh;

	(void)ac;
	(void)av;
	sh.env = copy_env(envp);
	sh.status = 0;
	if (!sh.env)
		return (1);
	setup_signals();
	line = read_line();
	while (line)
	{
		process_line(line, &sh);
		free(line);
		line = read_line();
	}
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	free_env(sh.env);
	return (sh.status);
}
