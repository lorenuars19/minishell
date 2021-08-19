/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redir_.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 17:58:33 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/19 18:28:46 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_o_flags(t_redirection *redir, int *o_flags, int file_exists)
{
	if (redir->mode == M_INPUT)
	{
		(*o_flags) = O_RDONLY;
	}
	else if (redir->mode == M_TRUNCATE)
		(*o_flags) = O_WRONLY | O_TRUNC;
	else if (redir->mode == M_APPEND)
		(*o_flags) = O_WRONLY | O_APPEND;
	else if (redir->mode == M_HEREDOC)
		*(o_flags) = O_TRUNC | O_WRONLY;
	if (!file_exists)
		*(o_flags) |= O_CREAT;
}

static int	sub_redir_file(t_exdat *ed, t_redirection *redir, int o_flags)
{
	if (redir->mode == M_HEREDOC)
	{
		ed->file_fd = open(HEREDOC_PATH, o_flags, 0664);
		if (ed->file_fd < 0)
			return (error_sys_put("set_redir_file : open"));
		get_here_document(ed->file_fd, redir->filename);
		close(ed->file_fd);
		ed->file_fd = open(HEREDOC_PATH, O_RDONLY, 0664);
		if (ed->file_fd < 0)
			return (error_sys_put("set_redir_file : open"));
	}
	else
	{
		ed->file_fd = open(redir->filename, o_flags, 0664);
		if (ed->file_fd < 0)
			return (error_sys_put("set_redir_file : open"));
	}
	return (0);
}

int	set_redir_file(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	t_redirection	*redir;
	int				o_flags;
	int				file_exists;

	redir = node->redirections;
	while (redir)
	{
		if (redir && ed->file_fd >= 0)
			close(ed->file_fd);
		o_flags = O_RDONLY;
		file_exists = is_path_regular_file(redir->filename);
		set_o_flags(redir, &o_flags, file_exists);
		if (sub_redir_file(ed, redir, o_flags))
			return (error_put(1, "set_redir_file : sub_redir_file"));
		if ((redir->mode == M_INPUT || redir->mode == M_HEREDOC))
			ctx->fd[STDIN_FILENO] = ed->file_fd;
		else
			ctx->fd[STDOUT_FILENO] = ed->file_fd;
		redir = redir->next;
	}
	return (0);
}

int	set_redir_dup(t_exdat *ed, t_node *node, t_ctx *ctx)
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
