#include <stdlib.h>
#include "minishell.h"

int	execution(t_node *node, char *envp[])
{
	t_ctx ctx;

	if (exec_nodes(node, &ctx, envp))
		return (1);

	return (0);
}

int	exec_nodes(t_node *node, t_ctx *ctx, char *envp[])
{
	if (node->type == COMMAND_NODE)
	{
		if (exec_command(node, envp))
			return (1);
	}
	else if (node->type == PIPE_NODE)
	{
		if (exec_piped(node, ctx, envp))
			return (1);
	}
	else
	{
		return (1);
	}
	return (0);
}

int	check_for_builtins(t_node *node, char *envp[])
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

int	exec_builtin(t_node *node, char *envp[], int index)
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

int	exec_command(t_node *node, char *envp[])
{
	int status;
	pid_t cpid;

	if (check_for_builtins(node, envp))
		return (1);
	cpid = fork();
	if (cpid < 0)
		return (error_sys_put(errno));
	else if (cpid == FORKED_CHILD)
		return (exec_binary(node, envp));
	else
	{
		//TODO parent stuff
		printf("Child PID : %d\n", cpid);
		status = wait_for_child(cpid);
		if (status)
		{
			return (error_printf(status, "child exit code : %d\n", status));
		}
		//TODO close pipe ? or other sutff maybe IDK
	}
	return (0);
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

int	exec_piped(t_node *node, t_ctx *ctx, char *envp[])
{
	if (node->type != PIPE_NODE)
		return (1);

	(void)ctx;
	(void)envp;

	return (0);
}

int	exec_binary(t_node *node, char *envp[])
{
	char	*path;
	int		status;

	status = 0;
	path = find_path(node);
	if (!path)
		return (error_put(1, "path is \033[7mNULL"));


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
