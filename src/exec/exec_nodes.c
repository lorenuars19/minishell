/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:02 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/17 21:44:11 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*
** The actual starting point that will be called recursively
*/

int	exec_nodes(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	if (node->type == COMMAND_NODE)
	{
		if (exec_command(ed, node, ctx))
			return (error_put(1, "exec_nodes : exec_command"));
	}
	else if (node->type == PIPE_NODE)
	{
		ed->is_pipe = TRUE;
		if (exec_pipe(ed, node, ctx))
			return (error_put(1, "exec_nodes : exec_pipe"));
	}
	else
		return (1);
	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_sys_put("setup_signals"));
	return (0);
}
