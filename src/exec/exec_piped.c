/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/15 15:30:19 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, char *envp[])
{
	t_exdat	lhs;
	t_exdat	rhs;

	ed->fd[READ_P] = ed->p[READ_P];
	ed->fd[WRIT_P] = ed->p[WRIT_P];

	if(ed->pipe_open == FALSE && pipe(ed->p))
		return (error_sys_put("pipe"));
	ed->pipe_open = TRUE;

	lhs = *ed;

	lhs.fd[READ_P] = STDIN_FILENO;
	lhs.fd_close = ed->p[WRIT_P];
	if (ed->pipe_open && ed->fd[READ_P] != -1)
	{
		lhs.fd[READ_P] = ed->fd[READ_P];
		lhs.fd_close = lhs.fd[READ_P];
	}

	lhs.fd[WRIT_P] = ed->p[WRIT_P];
	ed->status = exec_nodes(&lhs, node->left, envp);
	ed->n_children++;

	if (lhs.status)
		return (lhs.status);

	rhs = *ed;

	rhs.fd[READ_P] = ed->p[READ_P];

	rhs.fd[WRIT_P] = ed->p[WRIT_P];
	if ((node && node->right && node->right->type == COMMAND_NODE)
		|| (node && (!node->right)))
	{
		rhs.fd[WRIT_P] = STDOUT_FILENO;
	}

	rhs.fd_close = ed->p[READ_P];
	ed->status = exec_nodes(&rhs, node->right, envp);
	ed->n_children++;

	if (rhs.status)
		return (rhs.status);

	if (ed->p[READ_P] >= 0)
		close(ed->p[READ_P])

	if (ed->p[WRIT_P] >= 0)
		close(ed->p[WRIT_P]);

DED;

	return (ed->n_children);
}
