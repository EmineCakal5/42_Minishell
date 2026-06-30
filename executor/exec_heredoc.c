#include "../minishell.h"

int open_heredoc(char *delimiter)
{
    int fd;
    char *line;

    fd = open("/tmp/heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (-1);

    //heredoc okuma modu için sinyalleri sıfırklıyoruz
    reset_signals();

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    //okuma bitti doyayı başa sar ve döndür
    close(fd);
    fd = open("/tmp/heredoc_tmp", O_RDONLY);

    //normal sinyal davranışına dön
    setup_signals();
    return (fd);
}