#include "../minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (strcmp(cmd, "pwd") == 0 || strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "cd") == 0 || strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "export") == 0 || strcmp(cmd, "unset") == 0 ||
        strcmp(cmd, "exit") == 0 )
        return (1);
    return (0);
}

int run_builtin(t_cmd *cmd, char ***envp)
{
    (void)envp;

    if (strcmp(cmd->args[0], "pwd") == 0)
        return (ft_pwd());
    if (strcmp(cmd->args[0], "env") == 0)
        return (ft_env(*envp));
    if (strcmp(cmd->args[0], "exit") == 0)
        return (ft_exit(cmd));
    if (strcmp(cmd->args[0], "export") == 0)
        return (ft_export(cmd, envp));
    if (strcmp(cmd->args[0], "unset") == 0)
        return (ft_unset(cmd, envp));
    if (strcmp(cmd->args[0], "echo") == 0)
        return (ft_echo(cmd));
    if (strcmp(cmd->args[0], "cd") == 0)
        return (ft_cd(cmd, envp));
    return (0);
}