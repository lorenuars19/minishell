#include "minishell.h"

int	get_here_document(int fd, char *delimiter)
{
	char	*line;

	line = readline(">");
	while (line && str_cmp(line, delimiter) != 0)
	{
		write(fd, line, str_len(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	if (!line)
	{
		printf("line is null while getting here document\n");
		return (2);
	}
	free(line);
	return (0);
}
