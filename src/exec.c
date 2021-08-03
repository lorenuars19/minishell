#include "minishell.h"

int	execution(t_node *node)
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

int exec_command(t_node *node)
{
	if (node->type != COMMAND_NODE)
		return (1);

	if (fork == FORKED_CHILD)
}