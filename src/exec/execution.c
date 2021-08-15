/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 19:01:47 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/15 15:19:57 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <debug_utils.h>

static void	count_commands(t_exdat *ed, t_node *node)
{
	if (node && node->type == COMMAND_NODE)
	{
		ed->n_command++;
	}
	if (node && node->left)
	{
		count_commands(ed, node->left);
	}
	if(node && node->right)
	{
		count_commands(ed, node->right);
	}
}

int	execution(t_node *node, char *envp[])
{
	t_exdat	ed;

	ed = (t_exdat){0, FALSE, TRUE, builtin_dummy,
		FALSE, FALSE, {-1, -1}, {-1, -1}, -1, 0, 0};
	count_commands(&ed, node);

dprintf(2, "\n\n>-D-B-G->\n\033[33;1mn_command %d\033[0m\n<-D-B-G-<\n\n", ed.n_command);

	ed.status = exec_nodes(&ed, node, envp);

	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_put(ed.status, "execution : setup_signals : non-zero exit code"));
	if (ed.is_pipe == FALSE && ed.status)
		return (error_put(ed.status, "execution : exec_nodes : non-zero exit code"));

dprintf(2, "\n> exec_nodes : ed.n_children %d | ed.n_command %d <\n", ed.n_children, ed.n_command);

dprintf(2, "\n\n>-D-B-G->\n\033[33;1mn_children %d\033[0m\n<-D-B-G-<\n\n", ed.n_children);

	while (ed.n_children > 0)
	{
dprintf(2, "\n> exec_nodes : loop : before wait <\n");
		wait(&(ed.status));

//TODO remove debug
dprintf(2, "\n> exec_nodes : loop : child n %d terminated with status %x <\n",
				ed.n_children, ed.status);

		ed.n_children--;
	}
	if (ed.is_pipe == TRUE)
		return (get_exit_code(ed.status));

	return (ed.status);
}
