#include "minishell.h"

static void	exec_command_child(t_node *node, t_context *ctx)
{
	char	*binary_filename;

	if (set_redirection(node, ctx) != 0)
		exit(1);
	dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
	dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
	if (ctx->fd_close != -1)
		close(ctx->fd_close);
	if (is_command_a_builtin(node))
		exit(exec_builtin(node));
	if (!node->args[0])
		exit(0);
	binary_filename = get_bin_filename(node->args[0]);
	if (!binary_filename)
		exit(127);
	execve(binary_filename, node->args, g_shell.envp);
	print_error_filename(binary_filename);
	free(binary_filename);
	exit(errno);
}

static int	exec_command(t_node *node, t_context *ctx)
{
	pid_t	cpid;

	if (there_is_a_heredoc_redirection(node))
	{
		if (get_heredocs_redir(node) != 0)
			return (0);
	}
	if (is_command_a_builtin(node) && !is_part_of_pipeline(ctx))
		return (exec_builtin_in_parent(node, ctx));
	cpid = fork();
	if (cpid < 0)
	{
		put_str_fd(STDERR_FILENO, "minishell: fork: ");
		put_str_fd_nl(STDERR_FILENO, strerror(errno));
		return (errno);
	}
	else if (cpid == FORKED_CHILD)
		exec_command_child(node, ctx);
	node->pid = cpid;
	return (1);
}

static int exec_pipe(t_node *node, t_context *ctx)
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

int exec_node(t_node *node, t_context *ctx)
{
	int ret;

	ret = 0;
	if (node->type == COMMAND_NODE)
		return (exec_command(node, ctx));
	else if (node->type == PIPE_NODE)
		return (exec_pipe(node, ctx));
	return (ret);
}

int exec(t_node *node)
{
	t_context ctx;

	ctx.fd[STDIN_FILENO] = STDIN_FILENO;
	ctx.fd[STDOUT_FILENO] = STDOUT_FILENO;
	ctx.fd_close = -1;
	signal(SIGQUIT, sigquit_handler_exec);
	signal(SIGINT, sigint_handler_exec);
	exec_node(node, &ctx);
	wait_for_children(node, TRUE);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_handler_interactive);
	return (0);
}
