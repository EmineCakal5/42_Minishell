#include "../minishell.h"

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;

	//eğer ilk arguman "-n " ise , alt satıra geçme
	if (cmd->args[1] && strcmp(cmd->args[1], "-n") == 0)
	{
		new_line = 0;
		i = 2;
	}
	//argümanları yazdır
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
	return (0);
}