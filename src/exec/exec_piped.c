/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/13 13:49:41 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, char *envp[])
{
	t_exdat	lhs;
	t_exdat	rhs;

	if(ed->pipe_open == FALSE && pipe(ed->p))
		return (error_sys_put("pipe"));
	ed->pipe_open = TRUE;

//TODO REMOVE
// DED;

	lhs = *ed;

	lhs.fd[STDIN_FILENO] = STDIN_FILENO;

	lhs.fd[STDOUT_FILENO] = ed->p[STDOUT_FILENO];

	lhs.fd_close = ed->p[STDIN_FILENO];
	ed->status = exec_nodes(&lhs, node->left, envp);
	ed->n_children += lhs.n_children;

	if (lhs.status)
		return (lhs.status);

// usleep(1000 * 10000);

// DED;
	rhs = *ed;

	rhs.fd[STDIN_FILENO] = ed->p[STDIN_FILENO];

	rhs.fd[STDOUT_FILENO] = STDOUT_FILENO;

	rhs.fd_close = ed->p[STDOUT_FILENO];
	ed->status = exec_nodes(&rhs, node->right, envp);
	ed->n_children += rhs.n_children;

	if (rhs.status)
		return (rhs.status);

	if (ed->fd[STDIN_FILENO] >= 0 && close(ed->fd[STDIN_FILENO]))
		return (error_printf(errno, "exec_piped : close ed->fd[STDIN_FILENO] : %d %s",
				ed->fd[STDIN_FILENO], strerror(errno)));
	if (ed->fd[STDOUT_FILENO] >= 0 && close(ed->fd[STDOUT_FILENO]))
		return (error_printf(errno, "exec_piped : close ed->fd[STDOUT_FILENO] : %d %s",
				ed->fd[STDOUT_FILENO], strerror(errno)));
	return (ed->status);
}
