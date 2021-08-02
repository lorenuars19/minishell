#include "parsing.h"
#include <stdio.h>

t_bool	contains_unclosed_quotes(char *line)
{
	t_bool	in_quotes;
	char	delimiter;

	in_quotes = FALSE;
	while (*line)
	{
		if (in_quotes == TRUE && *line == delimiter)
			in_quotes = FALSE;
		else if (in_quotes == FALSE && (*line == '\'' || *line == '\"'))
		{
			in_quotes = TRUE;
			delimiter = *line;
		}
		line++;
	}
	if (in_quotes)
		return (TRUE);
	return (FALSE);
}

void	skip_blank_tokens(t_token **tokens)
{
	while (*tokens && (*tokens)->type == T_BLANK)
		(*tokens) = (*tokens)->next;
}

t_bool	contains_consecutive_pipes(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == T_PIPE)
		{
			tokens = tokens->next;
			skip_blank_tokens(&tokens);
			if (!tokens || tokens->type == T_PIPE)
				return (TRUE);
		}
		else
			tokens = tokens->next;
	}
	return (FALSE);
}

int syntax_checker(char *line, t_token *tokens)
{
	(void)tokens;
	if (contains_unclosed_quotes(line))
	{
		printf("There are unclosed quotes in line\n");
		return (1);
	}
	if (contains_consecutive_pipes(tokens))
	{
		printf("syntax error near '|'\n");
		return (1);
	}
	return (0);
}
