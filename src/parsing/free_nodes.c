#include "minishell.h"

static void free_args(char **args)
{
	int i;

	if (!args)
		return;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void free_redirections(t_redirection *redirections)
{
	if (redirections)
	{
		free_redirections(redirections->next);
		free(redirections->filename);
		free(redirections);
	}
}

void free_nodes(t_node *nodes)
{
	if (nodes->type == COMMAND_NODE)
	{
		free_args(nodes->args);
		free_redirections(nodes->redirections);
		free(nodes);
	}
	else if (nodes->type == PIPE_NODE)
	{
		free_nodes(nodes->left);
		free_nodes(nodes->right);
		free(nodes);
	}
}
