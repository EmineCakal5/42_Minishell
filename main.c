#include "minishell.h"

int	g_exit_status = 0;

static void	process_line(char *line, char ***env)
{
	t_token	*tokens;
	t_node	*ast;

	if (*line)
		add_history(line);
	tokens = tokenize(line);
	if (!tokens)
		return ;
	have_expand(tokens, *env);
	ast = parse(tokens);
	if (!ast)
		return ;
	if (prepare_heredocs(ast) == -1)
	{
		free_ast(ast);
		return ;
	}
	g_exit_status = execute(ast, env);
	free_ast(ast);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**shell_env;

	(void)ac;
	(void)av;
	shell_env = copy_env(envp);
	if (!shell_env)
		return (1);
	setup_signals();
	line = readline("minishell> ");
	while (line)
	{
		process_line(line, &shell_env);
		free(line);
		line = readline("minishell> ");
	}
	printf("exit\n");
	free_env(shell_env);
	return (0);
}
