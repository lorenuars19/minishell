/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 12:48:01 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 12:48:02 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error_filename(char *filename)
{
	put_str_fd(STDERR_FILENO, "minishell: ");
	put_str_fd(STDERR_FILENO, filename);
	put_str_fd(STDERR_FILENO, ": ");
	put_str_fd_nl(STDERR_FILENO, strerror(errno));
	return (1);
}

static int	set_input_redirection(t_redirection *redirection, t_context *ctx)
{
	int	fd;

	if (redirection->mode == M_INPUT)
	{
		if (ctx->fd[0] != STDIN_FILENO)
			close(ctx->fd[0]);
		fd = open(redirection->filename, O_RDONLY);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[0] = fd;
	}
	else if (redirection->mode == M_HEREDOC)
	{
		if (ctx->fd[0] != STDIN_FILENO)
			close(ctx->fd[0]);
		fd = open(HEREDOC_FILENAME, O_RDONLY);
		if (fd == -1)
			return (print_error_filename(HEREDOC_FILENAME));
		ctx->fd[0] = fd;
	}
	return (0);
}

static int	set_output_redirection(t_redirection *redirection, t_context *ctx)
{
	int	fd;

	if (redirection->mode == M_TRUNCATE)
	{
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
		fd = open(redirection->filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[1] = fd;
	}
	else if (redirection->mode == M_APPEND)
	{
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
		fd = open(redirection->filename, O_CREAT | O_APPEND | O_WRONLY, 0664);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[1] = fd;
	}
	return (0);
}

int	set_redirection(t_node *node, t_context *ctx)
{
	t_redirection	*current_redir;

	current_redir = node->redirections;
	while (current_redir)
	{
		if (current_redir->mode == M_INPUT || current_redir->mode == M_HEREDOC)
		{
			if (set_input_redirection(current_redir, ctx) != 0)
				return (1);
		}
		else if (current_redir->mode == M_TRUNCATE
			|| current_redir->mode == M_APPEND)
		{
			if (set_output_redirection(current_redir, ctx) != 0)
				return (1);
		}
		current_redir = current_redir->next;
	}
	return (0);
}
