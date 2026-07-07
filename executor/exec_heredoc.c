#include "../minishell.h"

static int	str_equal(const char *left, const char *right)
{
	while (*left && *right && *left == *right)
	{
		left++;
		right++;
	}
	return (*left == *right);
}

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

static int	build_heredoc_name(char *path, size_t size)
{
	size_t	index;
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
static char	*read_heredoc_line(void)
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

static int	read_heredoc_loop(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (g_exit_status == 130)
		{
			free(line);
			return (1);
		}
		if (!line)
			return (0);
		if (str_equal(line, delimiter))
		{
			free(line);
			return (0);
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int open_heredoc(char *delimiter)
{
	int					fd;
	int					cancelled;
	char				path[64];
	struct sigaction	old_int;

	if (build_heredoc_name(path, sizeof(path)) == -1)
		return (-1);
	fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	reset_signals(&old_int);
	g_exit_status = 0;
	cancelled = read_heredoc_loop(fd, delimiter);
	close(fd);
	if (cancelled)
	{
		unlink(path);
		restore_signals(&old_int);
		return (-2);
	}
	fd = open(path, O_RDONLY);
	unlink(path);
	restore_signals(&old_int);
	return (fd);
}
