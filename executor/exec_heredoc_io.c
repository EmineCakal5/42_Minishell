#include "../minishell.h"
#include "executor.h"

static size_t	append_number(char *path, size_t index, unsigned long value)
{
	char	buffer[32];
	size_t	len;

	len = 0;
	if (value == 0)
		buffer[len++] = '0';
	while (value > 0)
	{
		buffer[len++] = (char)('0' + (value % 10));
		value /= 10;
	}
	while (len > 0)
	{
		len--;
		path[index++] = buffer[len];
	}
	return (index);
}

int	build_heredoc_name(char *path, size_t size)
{
	size_t					index;
	static unsigned long	counter;

	if (size < 32)
		return (-1);
	index = 0;
	while ("/tmp/heredoc_"[index])
	{
		path[index] = "/tmp/heredoc_"[index];
		index++;
	}
	index = append_number(path, index, (unsigned long)getpid());
	path[index++] = '_';
	counter++;
	index = append_number(path, index, counter);
	path[index] = '\0';
	return (0);
}

static char	*grow_buf(char *buf, size_t old_cap, size_t new_cap)
{
	char	*new_buf;
	size_t	i;

	new_buf = malloc(new_cap);
	if (!new_buf)
	{
		free(buf);
		return (NULL);
	}
	i = 0;
	while (i < old_cap)
	{
		new_buf[i] = buf[i];
		i++;
	}
	free(buf);
	return (new_buf);
}

//readline yerine ham read() kullanıyoruz: Ctrl+C ile kesilince
//(SA_RESTART olmadan) EINTR net biçimde yakalanabiliyor, readline'ın
//iç durumuna bağlı belirsizlikler (karakter kaybı vb.) ortadan kalkıyor
char	*read_heredoc_line(void)
{
	char	*buf;
	size_t	cap;
	size_t	len;
	char	c;
	ssize_t	r;

	cap = 128;
	len = 0;
	buf = malloc(cap);
	if (!buf)
		return (NULL);
	write(STDOUT_FILENO, "> ", 2);
	while (1)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0)
		{
			if (len > 0)
				break ;
			free(buf);
			return (NULL);
		}
		if (c == '\n')
			break ;
		if (len + 1 >= cap)
		{
			buf = grow_buf(buf, cap, cap * 2);
			if (!buf)
				return (NULL);
			cap *= 2;
		}
		buf[len++] = c;
	}
	buf[len] = '\0';
	return (buf);
}
