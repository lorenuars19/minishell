#include "minishell.h"

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
		type = current_token->type;
		if (type == T_GREATER || type == T_SMALLER
			|| type == T_APPEND || type == T_HEREDOC)
			return (TRUE);
		current_token = current_token->next;
	}
	return (FALSE);
}

static char	*get_filename(t_token *token)
{
	if (has_redirection_type(token))
		token = token->next;
	if (token && token->data)
		return (token->data);
	return (NULL);
}

static int	get_one_redirection(t_token *current_token, t_node *node)
{
	t_redirection	*current_redir;

	current_redir = node->redirections;
	while (current_redir && current_redir->next)
		current_redir = current_redir->next;
	if (!node->redirections)
	{
		node->redirections = ft_calloc(1, sizeof(t_redirection));
		current_redir = node->redirections;
	}
	else
	{
		current_redir->next = ft_calloc(1, sizeof(t_redirection));
		current_redir = current_redir->next;
	}
	if (!current_redir)
		return (-1);
	current_redir->mode = (t_redirection_mode)current_token->type;
	if (current_redir->mode == M_HEREDOC
		&& current_token->next->type == T_GENERAL)
		current_redir->should_expand = TRUE;
	current_redir->filename = get_filename(current_token);
	return (0);
}

int	get_redirections(t_token *tokens, t_node *node)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token && current_token->type != T_PIPE)
	{
		if (has_redirection_type(current_token))
		{
			if (get_one_redirection(current_token, node) != 0)
				return (-1);
		}
		current_token = current_token->next;
	}
	return (0);
}
