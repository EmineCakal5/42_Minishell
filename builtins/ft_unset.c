#include "../minishell.h"

/*unset komutu, envp dizisini tarar,
 silinecek anahtarı bulur ve 
o elemanı diziden çıkartıp diziyi tekrar birleştirir.*/

int ft_unset(t_cmd *cmd, char ***envp)
{
    int i;
    int j;
    char **new_env;
    size_t len;

    i = 0;
    j = 0;
    len = strlen(cmd->args[1]);

    if (!cmd->args[1])
        return (0);

    while ((*envp)[i])
        i++;
    new_env = malloc(sizeof(char *) * i);

    i = 0;
    while ((*envp)[i])
    {
        //eğer değişken ismi eşleşmiyorsa kopyala
        if (!(strncmp((*envp)[i], cmd->args[1], len) == 0 && (*envp)[i][len] == '='))
            new_env[j++] = (*envp)[i];
        else
            free((*envp)[i]);
        i++;
    }
    new_env[j] = NULL;
    free(*envp);
    *envp = new_env;
    return (0);
}