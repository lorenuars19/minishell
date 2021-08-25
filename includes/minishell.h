/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 13:43:54 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 13:43:54 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include "libutils.h"

# include "parsing.h"
# include "exec.h"

# define HEREDOC_FILENAME "/tmp/.minishell_heredoc.txt"

typedef struct s_shell
{
	char	**envp;
	t_node	*nodes;
	int		last_exit_status;
}	t_shell;

extern t_shell	g_shell;

#endif
