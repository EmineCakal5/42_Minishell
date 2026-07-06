#include "minishell.h"

int	g_exit_status = 0;

static size_t	env_count(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

static char	**copy_env(char **envp)
{
	char	**copy;
	size_t	count;
	size_t	i;
	size_t	j;

	count = env_count(envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		j = 0;
		while (envp[i][j])
			j++;
		copy[i] = malloc(j + 1);
		if (!copy[i])
			return (NULL);
		j = 0;
		while (envp[i][j])
		{
			copy[i][j] = envp[i][j];
			j++;
		}
		copy[i][j] = '\0';
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	free_env(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_node	*ast;
	char	**shell_env;

	(void)ac;
	(void)av;
	shell_env = copy_env(envp);
	if (!shell_env)
		return (1);
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
		have_expand(tokens, shell_env);
		ast = parse(tokens);
		if (!ast)
			continue ;
		g_exit_status = execute(ast, &shell_env);

		free_ast(ast);
	}
	free_env(shell_env);
	return (0);
}
