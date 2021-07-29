#include "parsing.h"

t_bool	should_next_tokens_be_merged(t_token *tokens)
{
	t_token_type	type;
	t_token_type	next_type;

	if (tokens->next == NULL)
		return (FALSE);
	type = tokens->type;
	next_type = tokens->next->type;
	if	(type == T_GENERAL || type == T_DQUOTE || type == T_SQUOTE)
	{
		if (next_type == T_GENERAL || next_type == T_DQUOTE || next_type == T_SQUOTE)
			return (TRUE);
	}
	return (FALSE);
}

t_token	*merge_next_tokens(t_token	*tokens)
{
	return (tokens);
}

t_token	*merge_tokens(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;

	while (current_token)
	{
		if (should_next_tokens_be_merged(current_token))
		{
			merge_next_tokens(current_token);
		}
		current_token = current_token->next;
	}
	return (NULL);
}
