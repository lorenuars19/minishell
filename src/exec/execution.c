/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 19:01:47 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/19 14:07:32 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sub_wait_children(t_exdat *ed)
{
	int	wstatus;

	while (ed->n_children > 0)
	{
		if (waitpid(-1, &wstatus, WUNTRACED) < 0)
			break ;
		while (!WIFSIGNALED(wstatus) || !WIFEXITED(wstatus))
		{
			if (waitpid(-1, &wstatus, WUNTRACED) < 0)
				break ;
			if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus))
				break ;
		}
		if (ed->is_fork)
			ed->status = get_exit_code(wstatus);
		ed->n_children--;
	}
}

int	execution(t_node *node, char *envp[])
{
	t_ctx	ctx;
	t_exdat	ed;

	ed = (t_exdat){envp, 0, FALSE, TRUE, builtin_empty, FALSE, FALSE, 0, -1};
	ctx = (t_ctx){{STDIN_FILENO, STDOUT_FILENO}, -1};
	ed.status = exec_nodes(&ed, node, &ctx);
	if (setup_signals(REVERT_TO_DEFAULT))
		return (error_put(1, "execution : setup_signals : non-zero exit code"));
	if (ed.is_pipe == FALSE && ed.status)
		return (error_put(1, "execution : exec_nodes : non-zero exit code"));
	sub_wait_children(&ed);
	close(ed.file_fd);
	return (ed.status);
}
