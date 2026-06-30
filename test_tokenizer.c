#include "minishell.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = strlen(s);
	if (start >= s_len)
	{
		substr = malloc(1);
		if (substr)
			substr[0] = '\0';
		return (substr);
	}
	if (len > s_len - start)
		len = s_len - start;
	substr = (char *)malloc(len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

void	print_tokens(t_token *head)
{
	int	i;

	i = 1;
	while (head)
	{
		printf("[Token %d]\n", i);
		printf("  Type: ");
		if (head->type == WORD)
			printf("WORD\n");
		else if (head->type == PIPE)
			printf("PIPE\n");
		else if (head->type == REDIR_IN)
			printf("REDIR_IN\n");
		else if (head->type == REDIR_OUT)
			printf("REDIR_OUT\n");
		else if (head->type == APPEND)
			printf("APPEND\n");
		else if (head->type == HEREDOC)
			printf("HEREDOC\n");
		printf("  Value: \"%s\"\n\n", head->value);
		head = head->next;
		i++;
	}
}

int	main(void)
{
	t_token	*tokens;
	char	input1[] = "echo hello world";
	char	input2[] = "cat file.txt | grep test";
	char	input3[] = "ls -la > output.txt";

	printf("========== TEST 1 ==========\n");
	printf("Input: %s\n\n", input1);
	tokens = tokenize(input1);
	print_tokens(tokens);
	free_tokens(tokens);

	printf("\n========== TEST 2 ==========\n");
	printf("Input: %s\n\n", input2);
	tokens = tokenize(input2);
	print_tokens(tokens);
	free_tokens(tokens);

	printf("\n========== TEST 3 ==========\n");
	printf("Input: %s\n\n", input3);
	tokens = tokenize(input3);
	print_tokens(tokens);
	free_tokens(tokens);

	return (0);
}
