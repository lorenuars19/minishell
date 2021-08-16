/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 12:30:14 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	int		p[2];
	t_ctx	lhs;
	t_ctx	rhs;

	if(ed->pipe_open == FALSE && pipe(p))
		return (error_sys_put("pipe"));
	ed->pipe_open = TRUE;

	lhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};

	lhs.fd[READ_P] = STDIN_FILENO;
	lhs.fd_close = p[READ_P];
	if (ed->pipe_open && lhs.fd[READ_P] != -1)
	{
		lhs.fd[READ_P] = p[READ_P];
		lhs.fd_close = lhs.fd[READ_P];
	}

	lhs.fd[WRIT_P] = p[WRIT_P];
	ed->status = exec_nodes(ed, node->left, &rhs);

	rhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};

	rhs.fd[READ_P] = p[READ_P];
	rhs.fd[WRIT_P] = p[WRIT_P];
	if ((node && node->right && node->right->type == COMMAND_NODE)
		|| (node && (!node->right)))
	{
		rhs.fd[WRIT_P] = STDOUT_FILENO;
	}

	rhs.fd_close = p[WRIT_P];
	ed->status = exec_nodes(ed, node->right, &rhs);

	if (p[READ_P] >= 0)
		close(p[READ_P])

	if (p[WRIT_P] >= 0)
		close(p[WRIT_P]);

	return (ed->status);
}
