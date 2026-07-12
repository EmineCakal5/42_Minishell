#include "../minishell.h"

char	*append_str_simple(char *dst, const char *src)
{
	size_t	old_len;
	size_t	src_len;
	char	*tmp;

	if (dst)
		old_len = ft_strlen(dst);
	else
		old_len = 0;
	if (src)
		src_len = ft_strlen(src);
	else
		src_len = 0;
	tmp = ft_realloc(dst, old_len, old_len + src_len + 1);
	if (!tmp)
		return (NULL);
	if (src_len > 0)
		ft_memcpy(tmp + old_len, src, src_len);
	tmp[old_len + src_len] = '\0';
	return (tmp);
}

char	*append_substr_simple(char *dst, const char *src, int start, int len)
{
	size_t	old_len;
	char	*tmp;

	if (dst)
		old_len = ft_strlen(dst);
	else
		old_len = 0;
	tmp = ft_realloc(dst, old_len, old_len + len + 1);
	if (!tmp)
		return (NULL);
	if (len > 0)
		ft_memcpy(tmp + old_len, src + start, len);
	tmp[old_len + len] = '\0';
	return (tmp);
}

int	append_to_res(char **res, const char *src)
{
	char	*tmp;

	tmp = append_str_simple(*res, src);
	if (!tmp)
		return (1);
	*res = tmp;
	return (0);
}

int	append_char_to_res(char **res, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_to_res(res, buf));
}

int	append_range_to_res(char **res, char *value, int start, int len)
{
	char	*tmp;

	tmp = append_substr_simple(*res, value, start, len);
	if (!tmp)
		return (1);
	*res = tmp;
	return (0);
}
