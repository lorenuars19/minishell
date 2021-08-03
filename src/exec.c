#include <stdlib.h>
#include "minishell.h"

// int execution(t_node *node, char *envp[])
// {
// 	t_ctx ctx;

// 	if (exec_nodes(node, &ctx, envp))
// 		return (1);

// 	return (0);
// }

// int exec_nodes(t_node *node, t_ctx *ctx, char *envp[])
// {
// 	if (node->type == COMMAND_NODE)
// 	{
// 		if (exec_command(node, envp))
// 			return (1);
// 	}
// 	else if (node->type == PIPE_NODE)
// 	{
// 		if (exec_piped(node, ctx, envp))
// 			return (1);
// 	}
// 	else
// 	{
// 		return (1);
// 	}
// 	return (0);
// }

// int find_and_execute(t_node *node)
// {
// 	char *path;
// 	char **tab;

// 	path = getenv("PATH");

// 	tab = str_split(path, ":");

// 	for (int i = 0; tab && tab[i]; i++)
// 	{
// 		printf("paths tab[i %d] \"%s\"\n", i, tab[i]);
// 	}

// 	(void)node;

// 	// 		   V-> getcwd() CWD . ./
// 	// 1. execve(path, argv, envp)

// 	// >>

// 	// while (
// 	// 	exec (join(tab[i], ))
// 	// )

// 	return (0);
// }

// int check_for_builtins(t_node *node, char *envp[])
// {
// 	int			i;
// 	static char	*builtins[] = {
// 		"echo",
// 		"cd",
// 		"pwd",
// 		"export",
// 		"unset",
// 		"env",
// 		"exit",
// 		NULL
// 	};
// 	i = 0;
// 	while (node->args && node->args[0] && builtins[i]
// 		&& str_cmp(node->args[0], builtins[i]))
// 		i++;
// 	if (exec_builtin(node, envp, i))
// 		return (1);
// 	return (0);
// }

// int exec_builtin(t_node *node, char *envp[], int index)
// {
// 	static int (*builtins[])(char *argv[], char *envp[]) = {
// 		builtin_echo,
// 		builtin_cd,
// 		builtin_pwd,
// 		builtin_export,
// 		builtin_unset,
// 		builtin_env,
// 		builtin_exit
// 	};
// 	if (index >= 0 && index < BUILTIN_END)
// 	{
// 		if (builtins[index](node->args, envp))
// 			return (1);
// 	}
// 	return (0);
// }

// int exec_command(t_node *node, char *envp[])
// {
// 	int status;
// 	pid_t cpid;

// 	cpid = fork();
// 	if (cpid < 0)
// 	{
// 		return (1);
// 	}
// 	else if (cpid == FORKED_CHILD)
// 	{
// 		if (check_for_builtins(node, envp))
// 			return (1);
// 	}
// 	else
// 	{
// 		//TODO parent stuff
// 		status = wait_for_child(cpid);

// 		if (status)
// 			return (status);

// 		//TODO close pipe ? or other sutff maybe IDK
// 	}
// 	return (0);
// }

// int wait_for_child(pid_t cpid)
// {
// 	int wstatus;
// 	pid_t w;

// 	w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
// 	if (w == -1)
// 		return (-1);
// 	while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
// 	{
// 		w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
// 		if (WIFEXITED(wstatus))
// 			return (WEXITSTATUS(wstatus));
// 		else if (WIFSIGNALED(wstatus))
// 			return (WTERMSIG(wstatus));
// 		else if (WIFSTOPPED(wstatus))
// 			return (WSTOPSIG(wstatus));
// 	}
// 	return (0);
// }

// int exec_piped(t_node *node, t_ctx *ctx, char *envp[])
// {
// 	if (node->type != PIPE_NODE)
// 		return (1);

// 	DBM(exec_piped, node, ctx, ctx->fd[0], ctx->fd[1], ctx->close_fd, envp)

// 	return (0);
// }
