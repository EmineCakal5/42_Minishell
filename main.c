#include "minishell.h"

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_node	*ast;

	(void)ac;
	(void)av;
	(void)envp;
	setup_signals();


	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = tokenize(line);
		free(line);
		if (!tokens)
			continue;
		have_expand(tokens);
		ast = parse(tokens);
		if (!ast)
			continue ;
		printf("\n=== AST ===\n");
		print_ast(ast, 0);
		printf("\n");
		
		g_exit_status = execute(ast, envp);

		free_ast(ast);
	}
	return (0);
}
