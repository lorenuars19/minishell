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

static int	count_args(t_token *tokens)
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
