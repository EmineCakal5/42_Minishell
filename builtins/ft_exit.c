#include "../minishell.h"

static int	is_numeric_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	exit_code(char *arg)
{
	long long	value;
	int			sign;
	int			i;

	value = 0;
	sign = 1;
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	while (arg[i])
	{
		value = value * 10 + (arg[i] - '0');
		i++;
	}
	return ((int)(sign * value));
}

static void	print_exit_error(char *arg)
{
	write(2, "minishell: exit: ", 17);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
}
//Neden kullanıyoruz? 
//Kullanıcının shell'den düzgün bir şekilde ayrılmasını sağlamak zorunludur.
//Ne yapıyor? 
//exit() sistem çağrısını tetikler. cmd->args[1] kontrolü ile kullanıcının belirttiği kodu, yoksa mevcut g_exit_status değerini kullanır.

int	ft_exit(t_cmd *cmd)
{
	int	code;

	write(1, "exit\n", 5);
	if (cmd->args[1] && cmd->args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 35);
		g_exit_status = 1;
		return (1);
	}
	if (cmd->args[1])
	{
		if (!is_numeric_arg(cmd->args[1]))
		{
			print_exit_error(cmd->args[1]);
			exit(2);
		}
		code = exit_code(cmd->args[1]);
		exit((unsigned char)code);
	}
	exit(g_exit_status);
}