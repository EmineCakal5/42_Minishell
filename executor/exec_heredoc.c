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

int open_heredoc(char *delimiter)
{
    int fd;
    char *line;
    char path[64];

    if (build_heredoc_name(path, sizeof(path)) == -1)
        return (-1);
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (-1);

    //heredoc okuma modu için sinyalleri sıfırklıyoruz
    reset_signals();

    while (1)
    {
        line = readline("> ");
        if (!line || str_equal(line, delimiter))
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    //okuma bitti doyayı başa sar ve döndür
    close(fd);
    fd = open(path, O_RDONLY);
    unlink(path);

    //normal sinyal davranışına dön
    setup_signals();
    return (fd);
}