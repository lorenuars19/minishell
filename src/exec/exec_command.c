/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:58 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/19 16:45:30 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_ed(t_exdat *ed)
{
	ed->is_builtin = FALSE;
	ed->is_fork = TRUE;
	ed->f_to_call = builtin_empty;
}

static int	sub_exec_command_child(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	if (ed->is_fork == TRUE)
	{
		if (ed->is_pipe == TRUE && set_redir_pipe(ed, ctx, node))
			exit(error_put(1, "exec_command : set_redir_pipe"));
		if (node->redirections && set_redir_file(ed, node))
			exit(error_put(1, "exec_command : set_redir_file"));
	}
	if (ed->is_builtin == TRUE)
	{
		ed->status = ed->f_to_call(node->args, ed->envp);
		if (ed->status)
			return (error_put(ed->status, "sub_exec_command_child : builtin"));
	}
	else
	{
		if (exec_binary(node, ed->envp))
			return (error_put(1, "sub_exec_command_child : exec_binary"));
	}
	if (ed->is_fork == TRUE)
		exit(0);
	return (0);
}

int	exec_command(t_exdat *ed, t_node *node, t_ctx *ctx)
{
	int	cpid;

	if ((!node) || (node && node->type != COMMAND_NODE))
		return (error_put(1, "exec_command : node NULL or not COMMAND"));
	init_ed(ed);
	check_for_builtins(ed, node);
	ed->n_children++;
	cpid = 0;
	if (ed->is_fork == TRUE)
		cpid = fork();
	if (cpid < 0)
		return (error_sys_put("exec_command : fork"));
	else if (cpid == FORKED_CHILD)
	{
		if (sub_exec_command_child(ed, node, ctx))
			return (error_put(1, "exec_command : sub_exec_command_child"));
		return (0);
	}
	else
	{
		return (0);
	}
}
