#include "minishell.h"

int	execution(t_node *node, char *envp[])
{
	t_ctx ctx;

	exec_nodes(node, &ctx);

	return (0);
}

int exec_nodes(t_node *node, t_ctx * ctx)
{
	if (node->type == COMMAND_NODE)
	{
		if (exec_command(node))
			return (1);
	}
	else if (node->type == PIPE_NODE)
	{
		if (exec_piped(node))
			return (1);
	}
	else
	{
		return (1);
	}
	return (0);
}

int exec_command(t_node *node, char *envp[])
{
	int		cpid;
	char	*path;

	if (node->type != COMMAND_NODE)
		return (1);

	cpid = fork();
	if (cpid < 0)
	{
		return (1);
	}
	else if (cpid == FORKED_CHILD)
	{
		// TODO chidl stuff
		path = get_path(node);
		if (!path)
		{
			free(path);
			return (1);
		}
		int ret = execve(path, node->args, envp);
		if (ret < 0)
		{
			error_sys_put(errno);
		}
	}
	else
	{
		//TODO parent stuff
	}
	return(0);
}
