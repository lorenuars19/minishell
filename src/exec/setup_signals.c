/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorenuar <lorenuar@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 18:50:16 by lorenuar          #+#    #+#             */
/*   Updated: 2021/08/12 18:30:48 by lorenuar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sig_handle_child(int signum)
{
	kill(0, signum);
	setup_signals(REVERT_TO_DEFAULT);
}

static void	sig_handle_parent(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	setup_signals(t_revert revert_to_default)
{
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			return (error_sys_put("signal"));
	if (revert_to_default == REVERT_TO_DEFAULT)
	{
		if (signal(SIGINT, sig_handle_parent) == SIG_ERR)
			return (error_sys_put("signal"));
	}
	else
	{
		if (signal(SIGINT, sig_handle_child) == SIG_ERR)
			return (error_sys_put("signal"));
	}
	return (0);
}
