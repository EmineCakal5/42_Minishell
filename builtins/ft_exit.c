#include "../minishell.h"
//Neden kullanıyoruz? 
//Kullanıcının shell'den düzgün bir şekilde ayrılmasını sağlamak zorunludur.
//Ne yapıyor? 
//exit() sistem çağrısını tetikler. cmd->args[1] kontrolü ile kullanıcının belirttiği kodu, yoksa mevcut g_exit_status değerini kullanır.

int ft_exit(t_cmd *cmd)
{
    printf("exit\n");
    //eğer arg varsa (örn ext 1) o kodu kullan yoksa 0
    if (cmd->args[1])
        exit(ft_atoi(cmd->args[1]));
    exit(g_exit_status);
}