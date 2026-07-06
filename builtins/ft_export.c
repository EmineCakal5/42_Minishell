#include "../minishell.h"

static int	key_len(char *arg)
{
    int	i;

    i = 0;
    while (arg[i] && arg[i] != '=')
        i++;
    return (i);
}

static int	key_match(char *env, char *arg)
{
    int	i;
    int	len;

    i = 0;
    len = key_len(arg);
    while (i < len && env[i] && env[i] == arg[i])
        i++;
    return (i == len && env[i] == '=');
}

static int	env_count(char **envp)
{
    int	count;

    count = 0;
    if (!envp)
        return (0);
    while (envp[count])
        count++;
    return (count);
}

static char	*shell_strdup(char *s)
{
    char	*dup;
    int		i;

    i = 0;
    while (s[i])
        i++;
    dup = malloc(i + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
/*Burada char ***envp kullanmamızın sebebi; 
değişken eklediğimizde envp dizisinin bellekteki yerinin
 değişebilmesidir (realloc). Eğer sadece char envp gönderseydik,
  ana main fonksiyonundaki diziyi güncelleyemezdik.*/
int ft_export(t_cmd *cmd, char ***envp)
{
    char **new_env;
    char *new_entry;
    int i ;
    int count;

    i = 0;
    if (!envp || !*envp)
        return (0);
    if (!cmd->args[1])
    {
        //eğer argüman yoksa sadece mevcut envpyi listele(env ile aynı)
        ft_env(*envp);
        return (0);
    }
    new_entry = shell_strdup(cmd->args[1]);
    if (!new_entry)
        return (1);
    while ((*envp)[i])
    {
        if (key_match((*envp)[i], cmd->args[1]))
        {
            free((*envp)[i]);
            (*envp)[i] = new_entry;
            return (0);
        }
        i++;
    }
    count = env_count(*envp);
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(new_entry);
        return (1);
    }
    i = 0;
    while (i < count)
    {
        new_env[i] = (*envp)[i];
        i++;
    }
    new_env[i] = new_entry;
    new_env[i + 1] = NULL;
    free(*envp);
    *envp = new_env;
    return (0);
}