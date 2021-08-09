/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:45 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/09 19:00:42 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_piped(t_exdat *ed, t_node *node, char *envp[])
{
	if(pipe(ed->p))
		return (error_sys_put("pipe"));

//TODO REMOVE
dprintf(2, "ed->p[0] %d ed->p[1] %d\n", ed->p[0], ed->p[1]);

	ed->fd_close = ed->p[READ_P];
	ed->status = exec_nodes(ed, node->left, envp);
	if (ed->status)
		return (ed->status);
	ed->fd_close = ed->p[WRIT_P];
	ed->status = exec_nodes(ed, node->right, envp);
	if (ed->status)
		return (ed->status);
	if (close(ed->p[STDIN_FILENO]))
		return (error_sys_put("close"));
	if (close(ed->p[STDOUT_FILENO]))
		return (error_sys_put("close"));
	return (ed->status);
}
