/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredocs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aclose <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/25 12:53:19 by aclose            #+#    #+#             */
/*   Updated: 2021/08/25 12:53:19 by aclose           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_line_heredoc(char *line, t_bool should_expand)
{
	t_token	dummy;

	if (should_expand == FALSE)
		return (line);
	dummy.type = T_GENERAL;
	dummy.data = line;
	if (expand_in_one_token(&dummy, g_shell.envp) != 0)
		return (NULL);
	return (dummy.data);
}

static int	get_lines_heredoc(int fd, char *delimiter, t_bool should_expand)
{
	char	*line;

	line = readline("> ");
	while (line && str_cmp(line, delimiter) != 0)
	{
		line = get_line_heredoc(line, should_expand);
		if (!line)
			break ;
		put_str_fd_nl(fd, line);
		free(line);
		line = readline("> ");
	}
	if (!line)
		put_str_fd(STDERR_FILENO, "minishell: warning: here-document "
			"delimited by end-of-file (wanted `eof')\n");
	free(line);
	return (0);
}

static int	get_here_document(char *delimiter, t_bool should_expand)
{
	int	fd;

	fd = open(HEREDOC_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
	{
		put_str_fd(STDERR_FILENO, "minishell: heredoc: open: ");
		put_str_fd(STDERR_FILENO, strerror(errno));
		return (1);
	}
	get_lines_heredoc(fd, delimiter, should_expand);
	close(fd);
	return (0);
}

int	get_heredocs_redir(t_node *node)
{
	t_redirection	*redir;

	signal(SIGQUIT, SIG_IGN);
	redir = node->redirections;
	while (redir)
	{
		if (redir->mode == M_HEREDOC)
		{
			if (get_here_document(redir->filename, redir->should_expand) != 0)
			{
				signal(SIGQUIT, sigquit_handler_exec);
				return (1);
			}
		}
		redir = redir->next;
	}
	signal(SIGQUIT, sigquit_handler_exec);
	return (0);
}
