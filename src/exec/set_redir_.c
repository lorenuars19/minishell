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
		if (redir && ed->file_close >= 0)
			close(ed->file_close);
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
		ed->file_close = open(redir->filename, o_flags, (S_IRUSR + S_IWUSR)
			| (S_IRGRP + S_IWGRP) | S_IROTH);
		if (ed->file_close < 0)
			return (error_printf(errno, "set_redir_file : open : \"%s\" %d %s",
				redir->filename, ed->file_close, strerror(errno)));
		if (dup2(ed->file_close, dup_fd) < 0)
			return (error_printf(errno, "sub_set_redir : dup2 : %d %s",
				ed->file_close, strerror(errno)));
		redir = redir->next;
	}
	return (0);
}

int	set_redir_pipe(t_exdat *ed, t_ctx *ctx, t_node *node)
{
	if (!ed || !node || !ctx)
		return (1);

//TODO remove
dprintf(2, "\n> set_redirection : cmd [%s] | ed fd_to_close %d fd[0] %d fd[1] %d\n",
	node->args[0], ctx->fd_close, ctx->fd[0], ctx->fd[1]);

	if (ed->is_pipe == TRUE && ctx->fd[STDIN_FILENO] >= 0
		&& dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO) < 0)
		return (error_printf(errno, "set_redirection : dup2 STDIN : %d %s",
			ctx->fd[STDIN_FILENO], strerror(errno)));
	if (ed->is_pipe == TRUE && ctx->fd[STDOUT_FILENO] >= 0
		&& dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO) < 0)
		return (error_printf(errno, "set_redirection : dup2 STDOUT : %d %s",
			ctx->fd[STDOUT_FILENO], strerror(errno)));
	return (0);
}
