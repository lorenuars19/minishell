#include "minishell.h"

int	set_redir_file(t_exdat *ed, t_node *node)
{
	t_redirection	*redir;
	int				o_flags;
	int				dup_fd;
	int				file_exists;

	redir = node->redirections;
	while (redir)
	{
		if (redir && ed->fd_close >= 0)
			close(ed->fd_close);
		o_flags = O_RDONLY;
		dup_fd = STDOUT_FILENO;

		file_exists = is_path_regular_file(redir->filename);
		if (redir->mode == M_INPUT)
		{
			o_flags = O_RDONLY;
			dup_fd = STDIN_FILENO;
		}
		else if (redir->mode == M_TRUNCATE && !file_exists)
			o_flags = O_WRONLY | O_TRUNC | O_CREAT;
		else if (redir->mode == M_TRUNCATE && file_exists)
			o_flags = O_WRONLY;
		else if (redir->mode == M_APPEND && file_exists)
			o_flags =  O_WRONLY | O_APPEND;
		else if (redir->mode == M_APPEND && !file_exists)
			o_flags = O_WRONLY | O_APPEND | O_CREAT;
		ed->fd_close = open(redir->filename, o_flags, (S_IRUSR + S_IWUSR)
			| (S_IRGRP + S_IWGRP) | S_IROTH);
		if (ed->fd_close < 0)
			return (error_printf(errno, "set_redir_file : open : \"%s\" %d %s",
				redir->filename, ed->fd_close, strerror(errno)));
		if (dup2(ed->fd_close, dup_fd) < 0)
			return (error_printf(errno, "sub_set_redir : dup2 : %d %s",
				ed->fd_close, strerror(errno)));
		redir = redir->next;
	}
	return (0);
}

int	set_redir_pipe(t_exdat *ed, t_node *node)
{
	if (!ed || !node)
		return (1);

//TODO remove
dprintf(2, "\n> set_redirection : cmd [%s] | ed fd_to_close %d fd[0] %d fd[1] %d\n",
	node->args[0], ed->fd_close, ed->fd[0], ed->fd[1]);

	if (ed->is_pipe == TRUE && ed->fd[STDIN_FILENO] >= 0
		&& dup2(ed->fd[STDIN_FILENO], STDIN_FILENO) < 0)
		return (error_printf(errno, "set_redirection : dup2 STDIN : %d %s",
			ed->fd[STDIN_FILENO], strerror(errno)));
	if (ed->is_pipe == TRUE && ed->fd[STDOUT_FILENO] >= 0
		&& dup2(ed->fd[STDOUT_FILENO], STDOUT_FILENO) < 0)
		return (error_printf(errno, "set_redirection : dup2 STDOUT : %d %s",
			ed->fd[STDOUT_FILENO], strerror(errno)));
	if (ed->fd_close >= 0)
		close(ed->fd_close);
	return (0);
}
