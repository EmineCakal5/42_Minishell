#include "../minishell.h"

//pwd komutu, getcwd sistem çağrısını kullanarak mevcut çalışma dizinini
//ekrana yazar.

int	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}
