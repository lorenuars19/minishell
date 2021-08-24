#include "minishell.h"

static int fds_cleanup(t_context *ctx, int copy_fd[2])
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

	if (!node->args[0])
		return (FALSE);
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

int exec_builtin_in_parent(t_node *node, t_context *ctx)
{
	int copy_fd[2];

	if (set_redirection(node, ctx) != 0)
	{
		g_shell.last_exit_status = 1;
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
	g_shell.last_exit_status = exec_builtin(node);
	fds_cleanup(ctx, copy_fd);
	return (0);
}
