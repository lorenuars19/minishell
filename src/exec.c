#include "minishell.h"

//TODO check the case when there are only redirections but no command

void	wait_for_children(t_node *node, t_bool is_end_of_pipeline)
{
	int	wstatus;

	if (node->type == COMMAND_NODE && node->pid != 0)
	{
		if (waitpid(node->pid, &wstatus, 0) == -1)
			return ;
		if (WIFEXITED(wstatus) && is_end_of_pipeline)
			g_info.last_exit_status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus) && is_end_of_pipeline)
		{
			g_info.last_exit_status = WTERMSIG(wstatus) + 128;
			if (__WCOREDUMP(wstatus))
				put_str("Quit (core dumped)\n");
			else if (WTERMSIG(wstatus) == SIGINT)
				put_str("\n");
		}
	}
	else if (node->type == PIPE_NODE)
	{
		wait_for_children(node->left, FALSE);
		wait_for_children(node->right, TRUE);
	}
}

int	exec(t_node *node)
{
	t_context ctx;

	ctx.fd[STDIN_FILENO] = STDIN_FILENO;
	ctx.fd[STDOUT_FILENO] = STDOUT_FILENO;
	ctx.fd_close = -1;
	g_info.is_exec_ongoing = TRUE;
	exec_node(node, &ctx);
	wait_for_children(node, TRUE);
	g_info.is_exec_ongoing = FALSE;
	return (0);
}

int exec_node(t_node *node, t_context *ctx)
{
	int	ret;

	ret = 0;
	if (node->type == COMMAND_NODE)
		return (exec_command(node, ctx));
	else if (node->type == PIPE_NODE)
		return (exec_pipe(node, ctx));
	return (ret);
}

int	print_error_filename(char *filename)
{
	put_str_fd(STDERR_FILENO, "minishell: ");
	put_str_fd(STDERR_FILENO, filename);
	put_str_fd(STDERR_FILENO, ": ");
	put_str_fd_nl(STDERR_FILENO, strerror(errno));
	return (1);
}

int	set_input_redirection(t_redirection *redirection, t_context *ctx)
{
	int	fd;

	if (redirection->mode == M_INPUT)
	{
		if (ctx->fd[0] != STDIN_FILENO)
			close(ctx->fd[0]);
		fd = open(redirection->filename, O_RDONLY);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[0] = fd;
	}
	else if (redirection->mode == M_HEREDOC)
	{
		if (ctx->fd[0] != STDIN_FILENO)
			close(ctx->fd[0]);
		fd = open(HEREDOC_FILENAME, O_RDONLY);
		if (fd == -1)
			return (print_error_filename(HEREDOC_FILENAME));
		ctx->fd[0] = fd;
	}
	return (0);
}

int set_output_redirection(t_redirection *redirection, t_context *ctx)
{
	int fd;

	if (redirection->mode == M_TRUNCATE)
	{
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
		fd = open(redirection->filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[1] = fd;
	}
	else if (redirection->mode == M_APPEND)
	{
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
		fd = open(redirection->filename, O_CREAT | O_APPEND | O_WRONLY, 0664);
		if (fd == -1)
			return (print_error_filename(redirection->filename));
		ctx->fd[1] = fd;
	}
	return (0);
}

int	set_redirection(t_node *node, t_context *ctx)
{
	t_redirection	*current_redir;

	current_redir = node->redirections;
	while (current_redir)
	{
		if (current_redir->mode == M_INPUT
				|| current_redir->mode == M_HEREDOC)
		{
			if (set_input_redirection(current_redir, ctx) != 0)
				return (1);
		}
		else if (current_redir->mode == M_TRUNCATE
					|| current_redir->mode == M_APPEND)
		{
			if (set_output_redirection(current_redir, ctx) != 0)
				return (1);
		}
		current_redir = current_redir->next;
	}
	return (0);
}

t_bool	there_is_a_heredoc_redirection(t_node *node)
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

int		get_heredocs_redir(t_node *node)
{
	t_redirection *redir;

	redir = node->redirections;
	while (redir)
	{
		if (redir->mode == M_HEREDOC)
		{
			if (get_here_document(redir->filename) != 0)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

static char	*get_bin_filename_helper(char *name, char **paths)
{
	int			i;
	char		*bin_filename;
	struct stat	statbuf;

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

char	*get_bin_filename(char *name)
{
	char	*path_variable;
	char	**paths;
	char	*bin_filename;

	if (str_has(name, '/'))
		return (str_dupli(name));
	path_variable = get_value_from_envp("PATH", g_info.envp);
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

static t_bool	is_part_of_pipeline(t_context *ctx)
{
	if (ctx->fd[0] != STDIN_FILENO || ctx->fd[1] != STDOUT_FILENO)
		return (TRUE);
	return (FALSE);
}

int	fds_cleanup(t_context *ctx, int copy_fd[2])
{
	if (ctx->fd[0] != STDIN_FILENO)
	{
		close(ctx->fd[0]);
		dup2(copy_fd[0], STDIN_FILENO);
		close(copy_fd[0]);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		close(ctx->fd[1]);
		dup2(copy_fd[1], STDOUT_FILENO);
		close(copy_fd[1]);
	}
	return (0);
}

int	exec_builtin_in_parent(t_node *node, t_context *ctx)
{
	int	copy_fd[2];

	if (set_redirection(node, ctx) != 0)
	{
		g_info.last_exit_status = 1;
		return (0);
	}
	if (ctx->fd[0] != STDIN_FILENO)
	{
		copy_fd[0] = dup(STDIN_FILENO);
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
	}
	if (ctx->fd[1] != STDOUT_FILENO)
	{
		copy_fd[1] = dup(STDOUT_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
	}
	g_info.last_exit_status = exec_builtin(node);
	fds_cleanup(ctx, copy_fd);
	return (0);
}

int	exec_command(t_node *node, t_context *ctx)
{
	pid_t	cpid;
	char	*bin_filename;

	if (there_is_a_heredoc_redirection(node))
	{
		if (get_heredocs_redir(node) != 0)
			return (0);
	}
	if (is_command_a_builtin(node) && !is_part_of_pipeline(ctx))
	{
		exec_builtin_in_parent(node, ctx);
		return (0);
	}
	cpid = fork();
	if (cpid < 0)
	{
		put_str_fd(STDERR_FILENO, "minishell: fork: ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		return (0);
	}
	else if (cpid == FORKED_CHILD)
	{
		if (set_redirection(node, ctx) != 0)
			exit(1);
		// else
		// 	printf("%s\n", bin_filename);
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if (ctx->fd_close != -1)
			close(ctx->fd_close);
		if (is_command_a_builtin(node))
			exit(exec_builtin(node));
		bin_filename = get_bin_filename(node->args[0]);
		if (!bin_filename)
			exit(127);
		execve(bin_filename, node->args, g_info.envp);
		print_error_filename(bin_filename);
		free(bin_filename);
		exit(errno);
	}
	node->pid = cpid;
	return (1);
}

int exec_pipe(t_node *node, t_context *ctx)
{
	int			p[2];
	t_context	lhs_ctx;
	t_context	rhs_ctx;
	int			children;

	if (pipe(p) == -1)
	{
		put_str_fd(STDERR_FILENO, "minishell: pipe: ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		return (0);
	}
	lhs_ctx = *ctx;
	lhs_ctx.fd[STDOUT_FILENO] = p[STDOUT_FILENO];
	lhs_ctx.fd_close = p[STDIN_FILENO];
	children = exec_node(node->left, &lhs_ctx);

	close(p[STDOUT_FILENO]);

	rhs_ctx = *ctx;
	rhs_ctx.fd[STDIN_FILENO] = p[STDIN_FILENO];
	rhs_ctx.fd_close = -1;
	children += exec_node(node->right, &rhs_ctx);

	close(p[STDIN_FILENO]);
	return (children);
}

t_bool is_command_a_builtin(t_node *node)
{
	static char *builtin_names[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL};
	int i;

	i = 0;
	while (builtin_names[i])
	{
		if (str_cmp(node->args[0], builtin_names[i]) == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int exec_builtin(t_node *node)
{
	static int (*builtins[])(char *argv[]) = {
		builtin_echo, builtin_cd, builtin_pwd, builtin_export,
		builtin_unset, builtin_env, builtin_exit};
	static char *builtin_names[] = {
		"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL};
	int i;

	i = 0;
	while (builtin_names[i])
	{
		if (str_cmp(node->args[0], builtin_names[i]) == 0)
			return (builtins[i](node->args));
		i++;
	}
	return (-1);
}
