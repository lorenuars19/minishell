#include <stdlib.h>
#include <stdio.h>
#include "minishell.h"

t_bool	is_pipe_next(t_token *tokens)
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
		else if (has_redirection_type(tokens))
		{
			if (tokens->next)
				tokens = tokens->next;
			else
				return (count);
		}
		else if (tokens->type != T_BLANK)
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
		if (has_redirection_type(current_token))
			current_token = current_token->next;
		else if (current_token->type != T_BLANK)
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

t_bool	has_redirection_type(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (FALSE);
	type = token->type;
	if (type == T_GREATER || type == T_SMALLER
		|| type == T_APPEND || type == T_HEREDOC)
		return (TRUE);
	return (FALSE);
}

t_bool	contains_redirections(t_token *tokens)
{
	t_token			*current_token;
	t_token_type	type;

	if (!tokens)
		return (FALSE);
	current_token = tokens;
	while (current_token && current_token->type != T_PIPE)
	{
		//TODO need to think what to do about heredocs
		type = current_token->type;
		if (type == T_GREATER || type == T_SMALLER
			|| type == T_APPEND || type == T_HEREDOC)
			return (TRUE);
		current_token = current_token->next;
	}
	return (FALSE);
}

char	*get_filename(t_token *token)
{
	if (has_redirection_type(token))
		token = token->next;
	if (token && token->data)
		return (token->data);
	return (NULL);
}

void	get_redirections(t_token *tokens, t_node *node)
{
	t_token	*current_token;
	t_redirection	*current_redirection;

	//TODO check return value
	current_token = tokens;
	while (current_token && current_token->type != T_PIPE)
	{
		if (has_redirection_type(current_token))
		{
			if (!node->redirections)
			{
				node->redirections = ft_calloc(1, sizeof(t_redirection));
				current_redirection = node->redirections;
			}
			else
			{
				current_redirection->next = ft_calloc(1, sizeof(t_redirection));
				current_redirection = current_redirection->next;
			}
			// TODO fix compilation error
			current_redirection->mode = (t_redirection_mode)current_token->type;
			current_redirection->filename = get_filename(current_token);
		}
		current_token = current_token->next;
	}
	return ;
}

t_node	*parse_simple_command(t_token *tokens)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = COMMAND_NODE;
	if (contains_redirections(tokens))
		get_redirections(tokens, node);
	//TODO : check return value of get_args and get_redirections
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

static void	print_redirections(t_node *node)
{
	t_redirection	*current_redir;

	if (!node->redirections)
	{
		printf("NONE");
		return ;
	}
	current_redir = node->redirections;
	while (current_redir)
	{
		if (current_redir->next)
			printf("MODE: %c, FILENAME: %s, ", current_redir->mode, current_redir->filename);
		else
			printf("MODE: %c, FILENAME: %s", current_redir->mode, current_redir->filename);
		current_redir = current_redir->next;
	}
}

void	print_nodes(t_node *nodes, int spaces)
{
	if (nodes->type == COMMAND_NODE)
	{
		indent(spaces);
		printf("(");
		printf("ARGS: ");
		print_args(nodes);
		printf("; REDIRECTIONS: ");
		print_redirections(nodes);
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

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_redirections(t_redirection *redirections)
{
	if (redirections)
	{
		free_redirections(redirections->next);
		free(redirections->filename);
		free(redirections);
	}
}

void	free_nodes(t_node *nodes)
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
