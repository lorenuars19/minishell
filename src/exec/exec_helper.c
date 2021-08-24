#include "minishell.h"

void wait_for_children(t_node *node, t_bool is_end_of_pipeline)
{
	int wstatus;

	if (node->type == COMMAND_NODE && node->pid != 0)
	{
		if (waitpid(node->pid, &wstatus, 0) == -1)
			return;
		if (WIFEXITED(wstatus) && is_end_of_pipeline)
			g_shell.last_exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus) && is_end_of_pipeline)
		{
			g_shell.last_exit_status = WTERMSIG(wstatus) + 128;
			if (__WCOREDUMP(wstatus))
				printf("Quit (core dumped)\n");
			else if (WTERMSIG(wstatus) == SIGINT)
				printf("\n");
		}
	}
	else if (node->type == PIPE_NODE)
	{
		wait_for_children(node->left, FALSE);
		wait_for_children(node->right, TRUE);
	}
}

t_bool there_is_a_heredoc_redirection(t_node *node)
{
	t_redirection *redir;

	redir = node->redirections;
	while (redir)
	{
		if (redir->mode == M_HEREDOC)
			return (TRUE);
		redir = redir->next;
	}
	return (FALSE);
}

static char *get_bin_filename_helper(char *name, char **paths)
{
	int i;
	char *bin_filename;
	struct stat statbuf;

	i = 0;
	while (paths[i])
	{
		bin_filename = str_jointo(paths[i], "/", NULL);
		bin_filename = str_jointo(bin_filename, name, &bin_filename);
		if (!bin_filename)
			return (NULL);
		if (stat(bin_filename, &statbuf) == 0)
			return (bin_filename);
		free(bin_filename);
		i++;
	}
	return (NULL);
}

char *get_bin_filename(char *name)
{
	char *path_variable;
	char **paths;
	char *bin_filename;

	if (str_has(name, '/'))
		return (str_dupli(name));
	path_variable = get_value_from_envp("PATH", g_shell.envp);
	if (!path_variable)
		return (NULL);
	paths = ft_split(path_variable, ':');
	if (!paths)
		return (NULL);
	free(path_variable);
	bin_filename = get_bin_filename_helper(name, paths);
	free_envp(paths);
	if (!bin_filename)
	{
		put_str_fd(STDERR_FILENO, name);
		put_str_fd(STDERR_FILENO, ": command not found\n");
	}
	return (bin_filename);
}

t_bool is_part_of_pipeline(t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO || ctx->fd[1] != STDOUT_FILENO)
		return (TRUE);
	return (FALSE);
}

