/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 12:56:30 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 12:56:31 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler_interactive(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler_exec(int signum)
{
	(void)signum;
	return ;
}

void	sigquit_handler_exec(int signum)
{
	(void)signum;
	return ;
}

void	eof_exit(void)
{
	free_envp(g_shell.envp);
	put_str_fd_nl(STDERR_FILENO, "exit");
	exit(0);
}

t_bool	is_line_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
