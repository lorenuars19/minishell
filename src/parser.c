#include "parsing.h"
#include <stdlib.h>
#include <stdio.h>

bool	is_pipe_next(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == T_PIPE)
			return (TRUE);
		current_token = current_token->next;
	}
	return (FALSE);
}

int		count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			return (count);
		if (tokens->type != T_BLANK)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	**get_args(t_token *tokens)
{
	t_token	*current_token;
	int		args_count;
	char	**args;
	int		i;

	current_token = tokens;
	args_count = count_args(tokens);
	args = ft_calloc(args_count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (current_token && i < args_count)
	{
		if (current_token->type != T_BLANK)
		{
			args[i] = current_token->data;
			i++;
		}
		current_token = current_token->next;
	}
	return (args);
}

void	skip_tokens_until_next_command(t_token **tokens)
{
	t_token_type	type;

	while (*tokens)
	{
		type = (*tokens)->type;
		if (type == T_PIPE || (*tokens)->next == NULL)
		{
			*tokens = (*tokens)->next;
			return ;
		}
		*tokens = (*tokens)->next;
	}
}

t_node	*parse_simple_command(t_token *tokens)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = COMMAND_NODE;
	//TODO : check return value of get_args
	node->args = get_args(tokens);
	return (node);
}

t_node	*parse_pipe_command(t_token *tokens)
{
	t_node *node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = PIPE_NODE;
	//TODO : check all return values
	node->left = parse_simple_command(tokens);
	skip_tokens_until_next_command(&tokens);
	node->right = parser(tokens);
	return (node);
}

t_node	*parser(t_token *tokens)
{
	t_node	*nodes;

	if (!is_pipe_next(tokens))
	{
		nodes = parse_simple_command(tokens);
		return (nodes);
	}
	else
	{
		nodes = parse_pipe_command(tokens);
		return (nodes);
	}
}

static void	indent(int spaces)
{
	while (spaces > 0)
	{
		printf(" ");
		spaces--;
	}
}

static void	print_args(t_node *node)
{
	int		i;

	i = 0;
	while (node->args[i])
	{
		if (node->args[i + 1])
			printf("%s, ", node->args[i]);
		else
			printf("%s", node->args[i]);
		i++;
	}
}

void	print_nodes(t_node *nodes, int spaces)
{
	if (nodes->type == COMMAND_NODE)
	{
		indent(spaces);
		printf("COMMAND(");
		print_args(nodes);
		printf(")\n");
	}
	else if (nodes->type == PIPE_NODE)
	{
		indent(spaces);
		printf("PIPE(\n");
		print_nodes(nodes->left, spaces + 2);
		print_nodes(nodes->right, spaces + 2);
		indent(spaces);
		printf(")\n");
	}
}

static void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_nodes(t_node *nodes)
{
	if (nodes->type == COMMAND_NODE)
	{
		free_args(nodes->args);
		free(nodes);
	}
	else if (nodes->type == PIPE_NODE)
	{
		free_nodes(nodes->left);
		free_nodes(nodes->right);
		free(nodes);
	}
}
