/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:31 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/16 13:37:40 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_child(void)
{
	pid_t	w;
	int		ret;
	int		wstatus;

	w = waitpid(-1, &wstatus, 0);
	if (w < 0)
		return (error_sys_put("wait"));
	ret = get_exit_code(wstatus);
	if (ret)
		return (ret);
	return (ret);
}
