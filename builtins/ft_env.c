#include "../minishell.h"

//Bu fonksiyon, mevcut envp dizisini ekrana basar.
//Neden kullanıyoruz? 
//Shell'in o anki çalışma ortamını (path, user vb.) görmesi gerekir.
//Ne yapıyor? envp çift pointer'ı (char envp) 
//bir liste gibi gezerek her bir satırı terminale yazdırıyor.

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
	{
		write(1, envp[i], ft_strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
