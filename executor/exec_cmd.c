#include "../minishell.h"
#include "executor.h"

static void	free_path_list(char **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

static char	*join_path(char *left, char *right)
{
	char	*full_path;
	size_t	left_len;
	size_t	right_len;

	left_len = strlen(left);
	right_len = strlen(right);
	full_path = malloc(left_len + right_len + 2);
	if (!full_path)
		return (NULL);
	memcpy(full_path, left, left_len);
	full_path[left_len] = '/';
	memcpy(full_path + left_len + 1, right, right_len);
	full_path[left_len + right_len + 1] = '\0';
	return (full_path);
}

static char	**split_path_dirs(char *path_line)
{
	char	**paths;
	int		count;
	int		i;
	int		start;
	int		index;
	int		len;

	count = 1;
	i = 0;
	while (path_line[i])
	{
		if (path_line[i] == ':')
			count++;
		i++;
	}
	paths = calloc(count + 1, sizeof(char *));
	if (!paths)
		return (NULL);
	start = 0;
	index = 0;
	i = 0;
	while (1)
	{
		if (path_line[i] == ':' || path_line[i] == '\0')
		{
			len = i - start;
			paths[index] = malloc(len + 1);
			if (!paths[index])
			{
				free_path_list(paths);
				return (NULL);
			}
			memcpy(paths[index], path_line + start, len);
			paths[index][len] = '\0';
			index++;
			if (path_line[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	return (paths);
}

char	**get_path_dirs(char **envp)
{
	int		i;
	char	*path_line;

	/* PATH değişkenini ortamdan bulup ':' karakterine göre parçalara ayırıyoruz.
	   Böylece /bin, /usr/bin gibi dizinleri tek tek gezebiliyoruz. */
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (!strncmp(envp[i], "PATH=", 5))
		{
			/* "PATH=" kısmını atlayıp sadece gerçek dizin listesini alıyoruz. */
			path_line = envp[i] + 5;
			/* Dizinleri ':' ile ayırıp dizi halinde geri dönüyoruz. */
			return (split_path_dirs(path_line));
		}
		i++;
	}
	/* PATH yoksa komut adı üzerinden klasör taraması yapamayız. */
	return (NULL);
}

char	*join_cmd_path(char *dir, char *cmd)
{
	char	*full_path;

	/* Dizin ile komut adını tek bir PATH adayına çeviriyoruz. */
	full_path = join_path(dir, cmd);
	return (full_path);
}

char	*find_cmd_in_path(char **paths, char *cmd)
{
	int		i;
	char	*full_path;

	/* PATH içindeki her dizinde komutun çalıştırılabilir olup olmadığına bakıyoruz. */
	if (!paths || !cmd)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		/* Her dizini komut adıyla birleştirip tek tek deneme yolu oluşturuyoruz. */
		full_path = join_cmd_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		/* access(..., X_OK) komutun çalıştırılabilir olup olmadığını kontrol eder. */
		if (access(full_path, X_OK) == 0)
			/* Uygun bir yol bulunduysa artık bu yolu kullanabiliriz. */
			return (full_path);
		/* Bu yol çalışmıyorsa belleği temizleyip bir sonraki PATH dizinine geçiyoruz. */
		free(full_path);
		i++;
	}
	/* Hiçbir PATH dizininde komut bulunamadı. */
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;

	/* Bu fonksiyonun görevi: verilen komut adından çalıştırılabilir tam yol üretmek. */
	if (!cmd || !cmd[0])
		return (NULL);
	/* Komut içinde '/' varsa PATH araması yapmayız; kullanıcı zaten bir yol vermiştir. */
	if (strchr(cmd, '/'))
	{
		/* Verilen yol gerçekten çalıştırılabiliyorsa aynen kopyasını döndürüyoruz. */
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		/* Yol var ama çalıştırılamıyorsa NULL dönüyoruz. */
		return (NULL);
	}
	/* Komut sade isimse PATH değişkeninden dizinleri çıkarıyoruz. */
	paths = get_path_dirs(envp);
	if (!paths)
		return (NULL);
	/* PATH dizinlerini gezip komutu bulmaya çalışıyoruz. */
	cmd_path = find_cmd_in_path(paths, cmd);
	/* Split ile ayrılan PATH dizilerini serbest bırakıyoruz. */
	free_path_list(paths);
	/* Bulunan tam yolu, ya da bulunamadıysa NULL'u geri veriyoruz. */
	return (cmd_path);
}

int	exec_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*cmd_path;
	int		status;

	if (is_builtin(cmd->args[0]))
	{
		return (run_builtin(cmd, &envp));
	}
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	cmd_path = find_cmd_path(cmd->args[0], envp);
	if (!cmd_path)
	{
		printf("%s: command not found\n", cmd->args[0]);
		return (127);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		setup_signals_child();//sinyalleri varsayılana döndürür

		if (apply_redirections(cmd->redirs) == -1)
			exit(1);

		execve(cmd_path, cmd->args, envp);
		perror("execve");
		free(cmd_path);
		exit(126);
	}
	waitpid(pid, &status, 0);
	free(cmd_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}