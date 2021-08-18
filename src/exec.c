#include "minishell.h"

int	exec(t_node *node)
{
	t_context ctx;
	int		children;
	int		i;

	ctx.fd[STDIN_FILENO] = STDIN_FILENO;
	ctx.fd[STDOUT_FILENO] = STDOUT_FILENO;
	ctx.fd_close = -1;
	children = exec_node(node, &ctx);
	i = 0;
	while (i < children)
	{
		wait(NULL);
		i++;
	}
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

int	print_open_error(char *filename)
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
			return (print_open_error(redirection->filename));
		ctx->fd[0] = fd;
	}
	else if (redirection->mode == M_HEREDOC)
	{
		if (ctx->fd[0] != STDIN_FILENO)
			close(ctx->fd[0]);
		if (get_here_document(redirection->filename) != 0)
			return (1);
		fd = open(HEREDOC_FILENAME, O_RDONLY);
		if (fd == -1)
			return (print_open_error(HEREDOC_FILENAME));
		ctx->fd[0] = fd;
	}
	return (0);
}

int set_output_redirection(t_redirection *redirection, t_context *ctx)
{
	int fd;

	if (redirection->mode == M_TRUNCATE)
	{
		fd = open(redirection->filename, O_CREAT | O_TRUNC | O_WRONLY, 0664);
		if (fd == -1)
			return (print_open_error(redirection->filename));
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
		ctx->fd[1] = fd;
	}
	else if (redirection->mode == M_APPEND)
	{
		fd = open(redirection->filename, O_CREAT | O_APPEND | O_WRONLY, 0664);
		if (fd == -1)
			return (print_open_error(redirection->filename));
		if (ctx->fd[1] != STDOUT_FILENO)
			close(ctx->fd[1]);
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

int	exec_command(t_node *node, t_context *ctx)
{
	pid_t cpid;

	if (is_command_a_builtin(node))
	{
		exec_builtin(node);
		return (0);
	}
	if (set_redirection(node, ctx) != 0)
		return (1);
	cpid = fork();
	if (cpid < 0)
	{
		put_str_fd(STDERR_FILENO, "minishell: fork: ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		return (0);
	}
	else if (cpid == FORKED_CHILD)
	{
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if (ctx->fd_close != -1)
			close(ctx->fd_close);
		execvp(node->args[0], node->args);
		put_str_fd(STDERR_FILENO, "minishell: execve: ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		return (1);
	}
	if (ctx->fd[0] != STDIN_FILENO)
		close(ctx->fd[0]);
	if (ctx->fd[1] != STDOUT_FILENO)
		close(ctx->fd[1]);
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

int	sub_wait_for_child(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (wstatus);
}

int	wait_for_child(pid_t cpid)
{
	int ret;
	int wstatus;
	pid_t w;

	w = waitpid(cpid, &wstatus, WUNTRACED);
	if (w == -1)
		return (-1);
	ret = sub_wait_for_child(wstatus);
	if (ret)
		return (ret);
	while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
	{
		w = waitpid(cpid, &wstatus, WUNTRACED);
		ret = sub_wait_for_child(wstatus);
		if (ret)
			return (ret);
	}
	return (ret);
}


int	exec_binary(t_node *node, char *envp[])
{
	char	*path;
	int		status;

	status = 0;
	path = find_path(node);
	if (!path)
		return (error_put(1, "path is \033[7mNULL"));

	//TODO remove debug
	printf("\033[32;1mexecve\033[0m(path \"%s\", node->args <%p>, envp <%p>) \n\033[34m>->->->->->->->->\033[0m\n", path, node->args, envp);

	status = execve(path, node->args, envp);

	free(path);

	return (status);
}

char	*find_path(t_node *node)
{
	char	*path;
	char	**tab;
	int		i;

	if (node->args && node->args[0] && is_path_executable(node->args[0]))
		return (node->args[0]);
	path = getenv("PATH");
	tab = str_split(path, ":");
	i = 0;
	while (tab && tab[i] && node->args && node->args[0])
	{
		path = str_jointo(tab[i], "/", NULL);
		path = str_jointo(path, node->args[0], &path);
		if (is_path_executable(path))
		{
			return (path);
		}
		free(path);
		i++;
	}
	return (NULL);
}

int	is_path_executable(char *path)
{
	struct stat	sb;

	if (!path)
		return (0);

	// TODO test if path is an executable file
	if (stat(path, &sb))
		return (0);
	if (S_ISREG(sb.st_mode) && sb.st_mode & 0111)
	{
		return (1);
	}
	return (0);
}
