#include "minishell.h"

char	*get_line_heredoc(char *line, t_bool should_expand)
{
	t_token	dummy;

	if (should_expand == FALSE)
		return (line);
	dummy.type = T_GENERAL;
	dummy.data = line;
	if (expand_in_one_token(&dummy, g_info.envp) != 0)
		return (NULL);
	return (dummy.data);
}

int	get_lines_heredoc(int fd, char *delimiter, t_bool should_expand)
{
	char	*line;

	line = readline("> ");
	while (line && str_cmp(line, delimiter) != 0)
	{
		line = get_line_heredoc(line, should_expand);
		if (!line)
			break;
		put_str_fd_nl(fd, line);
		free(line);
		line = readline("> ");
	}
	if (!line)
		put_str_fd(STDERR_FILENO, "minishell: warning: here-document "
								  "delimited by end-of-file (wanted `eof')\n");
	free(line);
	return (0);
}

int	get_here_document(char *delimiter, t_bool should_expand)
{
	int	fd;

	fd = open(HEREDOC_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
	{
		put_str_fd(STDERR_FILENO, "minishell: heredoc: open: ");
		put_str_fd(STDERR_FILENO, strerror(errno));
		return (1);
	}
	get_lines_heredoc(fd, delimiter, should_expand);
	close(fd);
	return (0);
}
