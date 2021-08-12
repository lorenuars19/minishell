/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:49:58 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/12 17:29:22 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_command(t_exdat *ed, t_node *node, char *envp[])
{
	int	cpid;

	*ed = (t_exdat){0, FALSE, TRUE, builtin_dummy,
		ed->is_pipe, ed->pipe_open,
		{ed->p[0], ed->p[1]}, {ed->fd[0], ed->fd[1]}, ed->fd_close, 0};
	check_for_builtins(ed, node);

	cpid = 0;
	if (ed->is_fork == TRUE)
	{
		cpid = fork();

	}
	if (cpid < 0)
		return (error_sys_put("fork"));
	else if (cpid == FORKED_CHILD)
	{

DED;
		if (ed->is_fork == TRUE)
		{
			if (ed->is_pipe == TRUE && set_redir_pipe(ed, node))
				return(error_put(1, "exec_command : set_redir_pipe"));
			if (node->redirections && set_redir_file(ed, node))
				return(error_put(1, "exec_command : set_redir_file"));
		}
		if (ed->is_builtin == TRUE)
		{
			ed->status = ed->f_to_call(node->args, envp);
		}
		else
		{
			ed->status = exec_binary(node, envp);
		}
		if (ed->is_fork == TRUE)
			exit(ed->status);
		return (ed->status);
	}
	else
	{
		if (ed->is_pipe == FALSE)
		{
			ed->status = wait_for_child(cpid);
		}
		if (setup_signals(REVERT_TO_DEFAULT))
			return (error_sys_put("setup_signals"));
		if (ed->status)
			return (ed->status);
	}
	if (ed->is_pipe == TRUE)
		ed->n_children++;
	return (0);
}
