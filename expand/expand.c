#include "../minishell.h"

void	have_expand(t_token *t)
{
	t_token	*temp;
	char	*new_value;

	temp = t;
	while (temp)
	{
		if (!temp->value || temp->type != WORD)
		{
			temp = temp->next;
			continue ;
		}
		/* Heredoc delimiter'ı normal kelime gibi genişletmiyoruz. */
		if (temp->prev && temp->prev->type == HEREDOC)
		{
			temp = temp->next;
			continue ;
		}
		new_value = expand_value(temp->value);
		if (new_value)
		{
			free(temp->value);
			temp->value = new_value;
		}
		temp = temp->next;
	}
}
