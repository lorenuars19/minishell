#include "minishell.h"

static t_node	*parse_simple_command(t_token *tokens)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = COMMAND_NODE;
	if (contains_redirections(tokens))
	{
		if (get_redirections(tokens, node) != 0)
		{
			free_nodes(node);
			return (NULL);
		}
	}
	node->args = get_args(tokens);
	if (!node->args)
	{
		free_nodes(node);
		return (NULL);
	}
	return (node);
}

static t_node	*parse_pipe_command(t_token *tokens)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = PIPE_NODE;
	node->left = parse_simple_command(tokens);
	if (!node->left)
	{
		free_nodes(node);
		return (NULL);
	}
	skip_tokens_until_next_command(&tokens);
	node->right = parser(tokens);
	if (!node->right)
	{
		free_nodes(node);
		return (NULL);
	}
	return (node);
}

t_node	*parser(t_token *tokens)
{
	t_node	*nodes;

	if (!is_pipe_next(tokens))
		nodes = parse_simple_command(tokens);
	else
		nodes = parse_pipe_command(tokens);
	return (nodes);
}

int	parse(char *line)
{
	t_token	*tokens;

	tokens = scanner(line);
	if (syntax_checker(line, tokens) != 0)
	{
		free_tokens_incl_data(tokens);
		free(line);
		return (1);
	}
	free(line);
	if (expand_variables(g_shell.envp, tokens) != 0)
	{
		free_tokens_incl_data(tokens);
		return (1);
	}
	if (merge_tokens(tokens) != 0)
	{
		free_tokens_incl_data(tokens);
		return (1);
	}
	g_shell.nodes = parser(tokens);
	free_tokens_excl_data(tokens);
	if (!g_shell.nodes)
		return (1);
	return (0);
}
