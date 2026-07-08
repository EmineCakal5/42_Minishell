#include "../minishell.h"
#include "executor.h"

//AST'yi dolaşan ana fonk.
int	execute(t_node *node, char ***envp)
{
	if (!node)
		return (0);

	if (node->type == LEAF_PIPE)
	{
		exec_pipe(node, envp);
		return (g_exit_status);
	}
	else if (node->type == LEAD_CMD)
	{
		return (exec_cmd(node->cmd, envp));
	}
	return (0);
}