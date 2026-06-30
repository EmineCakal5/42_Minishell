#include "../minishell.h"

static void update_pwd_env(char ***envp, char *new_path)
{
    int i;
    char *new_entry;

    i = 0;
    new_entry = ft_strjoin("PWD=", new_path);
    if (!new_entry)
        return;
    
    while ((*envp)[i])
    {
        if (strncmp((*envp)[i], "PWD=", 4) == 0)
        {
            free((*envp)[i]);
            (*envp)[i] = new_entry;
            return;
        }
        i++;
    }
    free(new_entry);
}
int ft_cd(t_cmd *cmd, char ***envp)
{
    char cwd[1024];
    char *path;

    path = cmd->args[1];

    if (!path)
    {
        path = getenv("HOME");
        if (!path)
            return (0);
    }
    if (chdir(path) != 0)
    {
        perror("minshell: cd");
        return (1);
    }
    //dizin değişti pwd'yi güncelliyoruz
    if (getcwd(cwd, sizeof(cwd)))
    {
        update_pwd_env(envp, cwd);
    }
    return (0);
}