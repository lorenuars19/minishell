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

// void	get_simple_commands(t_token **tokens, t_node *nodes)
// {

// }

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

void parse_simple_command(t_token *tokens, t_node *node)
{
	node->type = COMMAND_NODE;
	node->args = get_args(tokens);
}

void	parse_pipe_command(t_token *tokens, t_node *node)
{
	node->type = PIPE_NODE;
	node->left = ft_calloc(1, sizeof(t_node));
	parse_simple_command(tokens, node->left);
	skip_tokens_until_next_command(&tokens);
	node->right = ft_calloc(1, sizeof(t_node));
	parse_simple_command(tokens, node->right);
}

t_node	*parser(t_token *tokens)
{
	t_node *nodes = ft_calloc(1, sizeof(t_node));
	if (!nodes)
		return (NULL);
	if (!is_pipe_next(tokens))
	{
		parse_simple_command(tokens, nodes);
		return (nodes);
	}
	else
	{
		parse_pipe_command(tokens, nodes);
		return (nodes);
	}
	return (nodes);
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
		printf("%s, ", node->args[i]);
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
