#include <stdlib.h>
#include "minishell.h"

int	execution(t_node *node, char *envp[])
{
	t_ctx ctx;

	if (exec_nodes(node, &ctx, envp))
		return (1);

	return (0);
}

int exec_nodes(t_node *node, t_ctx * ctx, char *envp[])
{
	if (node->type == COMMAND_NODE)
	{
		if (exec_command(node))
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

int find_and_execute(t_node *node)
{
	char *path;
	char **tab;

	path = getenv("PATH");

	tab = str_split(path, ":");

	for (int i = 0 ; tab && tab[i]; i++)
	{
		printf("paths tab[i %d] \"%s\"\n", i, tab[i]);
	}

	(void)node;

	return (0);
}

int exec_command(t_node *node)
{
	int		cpid;

	if (node->type != COMMAND_NODE)
		return (1);

	cpid = fork();
	if (cpid < 0)
	{
		return (1);
	}
	else if (cpid == FORKED_CHILD)
	{
		if (find_and_execute(node))
			return (1);
	}
	else
	{
		//TODO parent stuff
		//TODO close pipe ? or other sutff maybe IDK
	}
	return(0);
}


int exec_piped(t_node * node, t_ctx *ctx, char *envp[])
{
	if (node->type != PIPE_NODE)
		return(1);

DBM(EX_PIPE, node, ctx, ctx->fd[0], ctx->fd[1], ctx->close_fd, envp)

	return (0);
}
