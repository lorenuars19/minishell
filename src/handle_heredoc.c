#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int	get_here_document(char *delimiter)
{
	int	fd;
	char	*line;

	fd = open(HEREDOC_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
	{
		printf("problem while opening temporary heredoc file' %s\n", strerror(errno));
		return (1);
	}
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
	close(fd);
	return (0);
}
