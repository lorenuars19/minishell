/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 12:55:59 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 12:56:00 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	g_shell;

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argv;
	(void)argc;
	g_shell.envp = make_envp_copy(envp);
	if (!g_shell.envp)
		return (1);
	signal(SIGINT, sigint_handler_interactive);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("$>");
		if (!line)
			eof_exit();
		if (!is_line_empty(line))
			add_history(line);
		if (parse(line) != 0)
			continue ;
		exec(g_shell.nodes);
		free_nodes(g_shell.nodes);
	}
}
