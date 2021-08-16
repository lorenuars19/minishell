/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 15:12:52 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	close_and_return(t_exdat *ed, int p[2])
{
	if (p[READ_P] >= 0)
		close(p[READ_P])

	if (p[WRIT_P] >= 0)
		close(p[WRIT_P]);

	return (ed->status);
}

int	exec_piped(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	int		p[2];
	t_ctx	lhs;
	t_ctx	rhs;

	if (ctx->fd_close >= 0)
		close(ctx->fd_close);

	if(pipe(p))
		return (error_sys_put("pipe"));
	ed->pipe_open = TRUE;

DPCTX(ctx)


	lhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
DCTX(lhs)

	lhs.fd[READ_P] = STDIN_FILENO;
	lhs.fd[WRIT_P] = p[WRIT_P];
	lhs.fd_close = p[READ_P];
DCTX(lhs)
	ed->status = exec_nodes(ed, node->left, &lhs);
	if (ed->status)
		close_and_return(ed, p);



	rhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
DCTX(rhs)

	rhs.fd[READ_P] = p[READ_P];
	rhs.fd[WRIT_P] = p[WRIT_P];
	if ((node && node->right && node->right->type == COMMAND_NODE)
		|| (node && (!node->right)))
	{
		rhs.fd[WRIT_P] = STDOUT_FILENO;
	}

	rhs.fd_close = p[WRIT_P];
DCTX(rhs)
	ed->status = exec_nodes(ed, node->right, &rhs);
	if (ed->status)
		close_and_return(ed, p);
	return (0);
}
