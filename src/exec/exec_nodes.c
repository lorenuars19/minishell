/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:02 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/09 19:00:42 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*
** The actual starting point that will be called recursively
*/

int	exec_nodes(t_exdat *ed, t_node *node, char *envp[])
{
//TODO REMOvE
dprintf(2, "node->type %s | ed->p READ_P %d WRIT_P %d\n",
	(node->type == COMMAND_NODE) ? ("COMMAND_NODE") : ("PIPE_NODE"),
	ed->p[READ_P], ed->p[WRIT_P]);

	if (node->type == COMMAND_NODE)
	{
		ed->status = exec_command(ed, node, envp);
		if (ed->status)
			return (ed->status);
	}
	else if (node->type == PIPE_NODE)
	{
		ed->is_pipe = TRUE;
		ed->status = exec_piped(ed, node, envp);
		if (ed->status)
			return (ed->status);
	}
	else
	{
		return (1);
	}
	return (0);
}
