/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 18:34:43 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	int		p[2];
	t_ctx	lhs;
	t_ctx	rhs;

	close(ctx->fd_close);
	if(pipe(p))
		return (error_sys_put("pipe"));

dprintf(2, "\n>>\np[0] %d p[1] %d \n<<\n", p[0], p[1]);

	lhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
	lhs.fd[READ_P] = STDIN_FILENO;
	if (ctx->fd[READ_P] >= 0)
		lhs.fd[READ_P] = p[READ_P];
	lhs.fd[WRIT_P] = p[WRIT_P];
	lhs.fd_close = p[READ_P];
	exec_nodes(ed, node->left, &lhs);
	close(p[WRIT_P]);
	rhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
	rhs.fd[READ_P] = p[READ_P];
	rhs.fd[WRIT_P] = p[WRIT_P];
	if ((node && node->right && node->right->type == COMMAND_NODE)
		|| (node && (!node->right)))
		rhs.fd[WRIT_P] = STDOUT_FILENO;
	rhs.fd_close = p[WRIT_P];
	exec_nodes(ed, node->right, &rhs);
	close(p[WRIT_P]);
	close(p[READ_P]);
	return (0);
}
