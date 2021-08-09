/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 19:01:47 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/09 19:25:18 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execution(t_node *node, char *envp[])
{
	t_exdat	ed;

	ed = (t_exdat){0, FALSE, TRUE, builtin_dummy,
		FALSE, {-1, -1}, -1, 0};
	ed.status = exec_nodes(&ed, node, envp);
	if (ed.status)
		return (ed.status);
	while (ed.is_pipe == TRUE && ed.n_children >= 0)
	{
		wait(&(ed.status));

//TODO remove debug
dprintf(2, ">\n execution : loop : child terminated with status %x\n<",
	ed.status);

		ed.n_children--;
	}
	if (ed.is_pipe == TRUE)
		return (get_exit_code(ed.status));
	return (ed.status);
}
