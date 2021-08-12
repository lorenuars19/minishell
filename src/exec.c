#include <stdlib.h>
#include "minishell.h"

#include <debug_utils.h>

int	exec(t_node *node, char *envp[])
{
	t_context ctx;
	int		children;
	int		i;

	ctx.fd[STDIN_FILENO] = STDIN_FILENO;
	ctx.fd[STDOUT_FILENO] = STDOUT_FILENO;

DM(exec, ctx.fd[0])
DM(exec, ctx.fd[1])
DM(exec, ctx.fd_close)

	ctx.fd_close = -1;
	children = exec_node(node, &ctx, envp);
	printf("number of children spawned: %d\n", children);
	i = 0;
	while (i < children)
	{
		wait(NULL);
		i++;
	}
	return (0);
}

int exec_node(t_node *node, t_context *ctx, char *envp[])
{
	int	ret;

	ret = 0;
	if (node->type == COMMAND_NODE)
	{
		ret = exec_command(node, ctx, envp);
		return (ret);
	}
	else if (node->type == PIPE_NODE)
	{

DM(exec_node, ctx->fd[0])
DM(exec_node, ctx->fd[1])
DM(exec_node, ctx->fd_close)

		ret = exec_pipe(node, ctx, envp);
		return (ret);
	}
	return (ret);
}


int	exec_command(t_node *node, t_context *ctx, char *envp[])
{
	pid_t cpid;

	(void)envp;
	cpid = fork();
	if (cpid < 0)
	{
		put_str_fd_nl(STDERR_FILENO, "Error setting up the fork");
		return (-1);
	}
	else if (cpid == FORKED_CHILD)
	{


DM(exec_command before dup2s, ctx->fd[0])
DM(exec_command before dup2s, ctx->fd[1])
DM(exec_command before dup2s, ctx->fd_close)

		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO);
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO);
		if (ctx->fd_close != -1)
			close(ctx->fd_close);
		execvp(node->args[0], node->args);
		//TODO print on stderror
		printf("error: %s\n", strerror(errno));
	}
	return (1);
}

int exec_pipe(t_node *node, t_context *ctx, char *envp[])
{
	int	pipe_fd[2];
	t_node	*lhs;
	t_node	*rhs;
	t_context	lhs_ctx;
	t_context	rhs_ctx;
	int children;

	if (pipe(pipe_fd) == -1)
	{
		//TODO print on stderror
		printf("minishell: pipe: %s\n", strerror(errno));
		return (-1);
	}
	lhs_ctx = *ctx;
	lhs_ctx.fd[STDOUT_FILENO] = pipe_fd[STDOUT_FILENO];
	lhs_ctx.fd_close = pipe_fd[STDIN_FILENO];
	lhs = node->left;


DM(exec_pipe lhs, lhs_ctx.fd[0])
DM(exec_pipe lhs, lhs_ctx.fd[1])
DM(exec_pipe lhs, lhs_ctx.fd_close)

	children = exec_node(lhs, &lhs_ctx, envp);


	rhs_ctx = *ctx;
	rhs_ctx.fd[STDIN_FILENO] = pipe_fd[STDIN_FILENO];
	rhs_ctx.fd_close = pipe_fd[STDOUT_FILENO];
	rhs = node->right;

usleep(50000);

DM(exec_pipe rhs, rhs_ctx.fd[0])
DM(exec_pipe rhs, rhs_ctx.fd[1])
DM(exec_pipe rhs, rhs_ctx.fd_close)


	children += exec_node(rhs, &rhs_ctx, envp);

	close(pipe_fd[STDIN_FILENO]);
	close(pipe_fd[STDOUT_FILENO]);
	return (children);
}

// int	exec_command(t_node *node, char *envp[])
// {
// 	int status;
// 	pid_t cpid;

// 	if (check_for_builtins(node, envp))
// 		return (1);
// 	cpid = fork();
// 	if (cpid < 0)
// 		return (error_sys_put("fork"));
// 	else if (cpid == FORKED_CHILD)
// 		return (exec_binary(node, envp));
// 	else
// 	{
// 		//TODO parent stuff
// 		printf("Child PID : %d\n", cpid);
// 		status = wait_for_child(cpid);
// 		if (status)
// 		{
// 			return (error_printf(status, "child exit code : %d\n", status));
// 		}
// 		//TODO close pipe ? or other sutff maybe IDK
// 	}
// 	return (0);
// }

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

int check_for_builtins(t_node *node, char *envp[])
{
	int i;
	static char *builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		NULL};
	i = 0;
	while (node->args && node->args[0] && builtins[i] && str_cmp(node->args[0], builtins[i]))
		i++;
	if (exec_builtin(node, envp, i))
		return (1);
	return (0);
}

int exec_builtin(t_node *node, char *envp[], int index)
{
	static int (*builtins[])(char *argv[], char *envp[]) = {
		builtin_echo,
		builtin_cd,
		builtin_pwd,
		builtin_export,
		builtin_unset,
		builtin_env,
		builtin_exit};
	if (index >= 0 && index < BUILTIN_END)
	{
		if (builtins[index](node->args, envp))
		{
			return (1);
		}
	}
	return (0);
}
