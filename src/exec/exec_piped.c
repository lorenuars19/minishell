/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/10 01:47:19 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, char *envp[])
{
	t_exdat	lhs;
	t_exdat	rhs;

	if(pipe(ed->p))
		return (error_sys_put("pipe"));

//TODO REMOVE
dprintf(2, "ed->p[0] %d ed->p[1] %d\n", ed->p[0], ed->p[1]);

	lhs = *ed;
	lhs.p[STDOUT_FILENO] = ed->p[STDOUT_FILENO];
	lhs.fd_close = ed->p[STDIN_FILENO];
	ed->status = exec_nodes(&lhs, node->left, envp);

	if (ed->status)
		return (ed->status);



	rhs = *ed;
	rhs.p[STDIN_FILENO] = ed->p[STDIN_FILENO];
	rhs.fd_close = ed->p[STDOUT_FILENO];
	ed->status = exec_nodes(&rhs, node->right, envp);

	if (ed->status)
		return (ed->status);



	if (close(ed->p[STDIN_FILENO]))
		return (error_sys_put("close"));
	if (close(ed->p[STDOUT_FILENO]))
		return (error_sys_put("close"));
	return (ed->status);
}
