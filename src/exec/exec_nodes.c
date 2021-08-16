/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:02 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 20:07:04 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*
** The actual starting point that will be called recursively
*/

int exec_nodes(t_exdat *ed, t_node *node, t_ctx *ctx)
{

	if (node->type == COMMAND_NODE)
	{
		ed->status = exec_command(ed, node, ctx);
		if (ed->status)
			return (ed->status);
	}
	else if (node->type == PIPE_NODE)
	{
		ed->is_pipe = TRUE;
		ed->status = exec_pipe(ed, node, ctx);
		if (ed->status)
			return (ed->status);
	}
	else
	{
		return (1);
	}
	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_sys_put("setup_signals"));
	return (ed->status);
}