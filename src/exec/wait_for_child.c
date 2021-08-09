/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:31 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/09 19:22:41 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_child(pid_t cpid)
{
	pid_t	w;
	int		ret;
	int		wstatus;

	w = waitpid(cpid, &wstatus, WUNTRACED);
	if (w < 0)
		return (error_sys_put("wait"));
	ret = get_exit_code(wstatus);
	if (ret)
		return (ret);
	while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
	{
		w = waitpid(cpid, &wstatus, WUNTRACED);
		ret = get_exit_code(wstatus);
		if (ret)
			return (ret);
	}
	return (ret);
}
