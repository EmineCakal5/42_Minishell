#include "../minishell.h"
/*Burada char ***envp kullanmamızın sebebi; 
değişken eklediğimizde envp dizisinin bellekteki yerinin
 değişebilmesidir (realloc). Eğer sadece char envp gönderseydik,
  ana main fonksiyonundaki diziyi güncelleyemezdik.*/
int ft_export(t_cmd *cmd, char ***envp)
{
    char **new_env;
    int i ;

    i = 0;
    if (!cmd->args[1])
    {
        //eğer argüman yoksa sadece mevcut envpyi listele(env ile aynı)
        ft_env(*envp);
        return (0);
    }
    // 1- Mevcut envp uzunluğunu bul
    while ((*envp)[i])
        i++;
    //2- Yeni dizi için yer ayır (mevcut + 1 yeni + null)
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return (1);
    //3- Eskileri kopyala
    i = 0;
    while ((*envp)[i])
    {
        new_env[i] = (*envp)[i];
        i++;
    }
    //4- Yeni değişkeni ekle
    new_env[i] = strdup(cmd->args[1]);
    new_env[i+1] = NULL;
    //5- E ski diziyi serbest bırak ve envpyi güncelle
    free(*envp);
    *envp = new_env;
    
    return (0);
}