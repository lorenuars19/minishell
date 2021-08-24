#include "minishell.h"

static t_bool	should_next_tokens_be_merged(t_token *tokens)
{
	t_token_type	type;
	t_token_type	next_type;

	if (tokens->next == NULL)
		return (FALSE);
	type = tokens->type;
	next_type = tokens->next->type;
	if	(type == T_GENERAL || type == T_DQUOTE || type == T_SQUOTE)
	{
		if (next_type == T_GENERAL || next_type == T_DQUOTE
				|| next_type == T_SQUOTE)
			return (TRUE);
	}
	return (FALSE);
}

static t_token	*merge_next_tokens(t_token	*tokens)
{
	char	*new_data;
	int		i;
	t_token	*new_next;

	new_data = ft_calloc(str_len(tokens->data)
							+ str_len(tokens->next->data) + 1
						, sizeof(char));
	if (!new_data)
		return (NULL);
	i = ft_strncpy(new_data, tokens->data, str_len(tokens->data));
	ft_strncpy(new_data + i, tokens->next->data, str_len(tokens->next->data));
	free(tokens->data);
	free(tokens->next->data);
	new_next = tokens->next->next;
	free(tokens->next);
	tokens->next = new_next;
	tokens->data = new_data;
	return (tokens);
}

static void	remove_blank_tokens(t_token *tokens)
{
	t_token *current_token;
	t_token *temp;

	current_token = tokens;
	while (current_token && current_token->next)
	{
		if (current_token->next->type == T_BLANK)
		{
			temp = current_token->next->next;
			free(current_token->next);
			current_token->next = temp;
		}
		current_token = current_token->next;
	}
}

int merge_tokens(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		if (should_next_tokens_be_merged(current_token))
		{
			current_token = merge_next_tokens(current_token);
			if (!current_token)
				return (-1);
		}
		else
			current_token = current_token->next;
	}
	remove_blank_tokens(tokens);
	return (0);
}
