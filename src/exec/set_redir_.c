#include "minishell.h"

static void set_o_flags(t_redirection *redir, int *o_flags,
						int file_exists, int *dup_fd)
{
	if (redir->mode == M_INPUT)
	{
		(*o_flags) = O_RDONLY;
		*(dup_fd) = STDIN_FILENO;
	}
	else if (redir->mode == M_TRUNCATE)
		(*o_flags) = O_WRONLY | O_TRUNC;
	else if (redir->mode == M_APPEND)
		(*o_flags) = O_WRONLY | O_APPEND;
	else if (redir->mode == M_HEREDOC)
		*(o_flags) = O_TMPFILE | O_RDWR;
	if (!file_exists)
		*(o_flags) |= O_CREAT;
}

int set_redir_file(t_exdat *ed, t_node *node)
{
	t_redirection *redir;
	int o_flags;
	int dup_fd;
	int file_exists;

	redir = node->redirections;
	while (redir)
	{
		if (redir && ed->file_fd >= 0)
			close(ed->file_fd);
		o_flags = O_RDONLY;
		dup_fd = STDOUT_FILENO;
		file_exists = is_path_regular_file(redir->filename);
		set_o_flags(redir, &o_flags, file_exists, &dup_fd);
dprintf(2, "redir->filename %s\n", redir->filename);
		if (redir->mode == M_HEREDOC)
		{
			ed->file_fd = open(HEREDOC_PATH, o_flags, 0664);
dprintf(2, "ed->file_fd %d\n", ed->file_fd);
			get_here_document(ed->file_fd, redir->filename);
		}
		else
		{
			ed->file_fd = open(HEREDOC_PATH, o_flags, 0664);
			if (ed->file_fd < 0)
				return (error_sys_put("set_redir_file : open"));
		}
		if (dup2(ed->file_fd, dup_fd) < 0)
			return (error_sys_put("sub_set_redir : dup2"));
		redir = redir->next;
	}
	return (0);
}

int set_redir_pipe(t_exdat *ed, t_ctx *ctx, t_node *node)
{
	if (!ed || !node || !ctx)
		return (1);
	if (ctx->fd[STDIN_FILENO] != STDIN_FILENO
		&& dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO) < 0)
		return (error_sys_put("set_redirection : dup2 STDIN"));
	if (ctx->fd[STDOUT_FILENO] != STDOUT_FILENO
		&& dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO) < 0)
		return (error_sys_put("set_redirection : dup2 STDOUT"));
	if (ctx->fd_close >= 0)
		close(ctx->fd_close);
	return (0);
}
