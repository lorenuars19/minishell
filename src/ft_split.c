#include "minishell.h"

static int	ft_count_words(char const *s, char c)
{
	int	inside_a_word;
	int	word_count;
	int	i;

	i = 0;
	inside_a_word = 0;
	word_count = 0;
	while (s[i])
	{
		if (s[i] != c && inside_a_word == 0)
		{
			word_count++;
			inside_a_word = 1;
		}
		else if (s[i] == c)
			inside_a_word = 0;
		i++;
	}
	return (word_count);
}

static size_t	ft_word_length(char const *s, char c, int index)
{
	int	i;

	i = 0;
	while (s[index + i] && s[index + i] != c)
		i++;
	return (i);
}

static void	destroy_strs(char **strs, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(strs[j++]);
	free(strs);
}

static char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t i;
	char *res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (start <= str_len(s))
	{
		while (i < len && s[start + i])
		{
			res[i] = s[start + i];
			i++;
		}
	}
	res[i] = '\0';
	return (res);
}

static char	**fill_strs(char **strs, char const *s, char c, int word_count)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	while (i < word_count)
	{
		while (s[j] == c)
			j++;
		strs[i] = ft_substr(s, j, ft_word_length(s, c, j));
		if (strs[i] == NULL)
		{
			destroy_strs(strs, i);
			return (NULL);
		}
		j += ft_word_length(s, c, j);
		i++;
	}
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = ft_count_words(s, c);
	strs = ft_calloc((word_count + 1), sizeof(char *));
	if (!strs)
		return (NULL);
	if (!fill_strs(strs, s, c, word_count))
		return (NULL);
	return (strs);
}
