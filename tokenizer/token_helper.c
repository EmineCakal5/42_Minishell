#include "../minishell.h"

t_type	op_type(char *s, int i)
{
	int	len;

	len = op_number(s, i);
	if (s[i] == '|' && len == 1)
		return (PIPE);
	else if (s[i] == '<' && len == 1)
		return (REDIR_IN);
	else if (s[i] == '<' && len == 2)
		return (HEREDOC);
	else if (s[i] == '>' && len == 1)
		return (REDIR_OUT);
	else if (s[i] == '>' && len == 2)
		return (APPEND);
	return (WORD);
}

int	is_empty(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	op(char a)
{
	if (a == '<' || a == '|' || a == '>')
		return (1);
	return (0);
}

int	op_number(char *str, int i)
{
	if ((str[i] == '<' && str[i + 1] == '<')
		|| (str[i] == '>' && str[i + 1] == '>'))
		return (2);
	if (str[i] == '<' || str[i] == '>' || str[i] == '|')
		return (1);
	return (0);
}
