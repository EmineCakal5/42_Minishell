#include "../minishell.h"

//pwd komutu, getcwd sistem çağrısını kullanarak mevcut çalışma dizinini ekrana yazar.

int ft_pwd(void)
{
	char cwd[1024]; //dizin yolunu tutacak buffer

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}