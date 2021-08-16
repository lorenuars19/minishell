/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 19:01:47 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 16:25:15 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <debug_utils.h>

static void sub_wait_children(t_exdat *ed)
{
	int		wstatus;
	pid_t	cpid;

	while (ed->n_children > 0)
	{
dprintf(2, "\n> exec_nodes : loop : before wait <\n");

	cpid = waitpid(-1, &wstatus, WUNTRACED);
	while (!WIFSIGNALED(wstatus) || !WIFEXITED(wstatus))
	{
		cpid = waitpid(-1, &wstatus, WUNTRACED);
		if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus))
			break;
	}
	ed->status = get_exit_code(wstatus);

//TODO remove debug
dprintf(2, "\n> exec_nodes : loop : child pid %d nbr %d terminated with status %x <\n",
				cpid, ed->n_children, wstatus);

		ed->n_children--;
	}
}

int	execution(t_node *node, char *envp[])
{
	t_ctx	ctx;
	t_exdat	ed;

	ed = (t_exdat){envp, 0, FALSE, TRUE, builtin_dummy, FALSE, FALSE, 0, -1};
	ctx = (t_ctx){{STDIN_FILENO, STDOUT_FILENO}, -1};

	ed.status = exec_nodes(&ed, node, &ctx);

	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_put(ed.status, "execution : setup_signals : non-zero exit code"));
	if (ed.is_pipe == FALSE && ed.status)
		return (error_put(ed.status, "execution : exec_nodes : non-zero exit code"));

dprintf(2, "\n\n>-DBG->\n\033[33;1mn_children %d\033[0m\n<-DBG-<\n\n", ed.n_children);

	sub_wait_children(&ed);

	return (ed.status);
}
