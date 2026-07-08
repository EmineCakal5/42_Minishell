#include "../minishell.h"
#include "executor.h"

static char	**get_path_dirs(char **envp)
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

static char	*join_cmd_path(char *dir, char *cmd)
{
	char	*full_path;

	/* Dizin ile komut adını tek bir PATH adayına çeviriyoruz. */
	full_path = join_path(dir, cmd);
	return (full_path);
}

static char	*find_cmd_in_path(char **paths, char *cmd)
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
