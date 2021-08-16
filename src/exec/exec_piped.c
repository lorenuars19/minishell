/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 20:02:37 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	int		p[2];
	t_ctx	lhs;
	t_ctx	rhs;

	if(pipe(p))
		return (error_sys_put("pipe"));
	lhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
	lhs.fd[STDOUT_FILENO] = p[STDOUT_FILENO];
	lhs.fd_close = p[READ_P];
	exec_nodes(ed, node->left, &lhs);
	close(p[WRIT_P]);
	rhs = (t_ctx){{ctx->fd[0], ctx->fd[1]}, ctx->fd_close};
	rhs.fd[STDIN_FILENO] = p[STDIN_FILENO];
	if ((node && node->right && node->right->type == COMMAND_NODE)
		|| (node && (!node->right)))
		rhs.fd[WRIT_P] = STDOUT_FILENO;
	rhs.fd_close = p[WRIT_P];
	exec_nodes(ed, node->right, &rhs);
	close(p[READ_P]);
	return (0);
}
