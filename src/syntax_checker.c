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

t_bool	token_contains_a_string(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (FALSE);
	type = token->type;
	if (type == T_GENERAL || type == T_DQUOTE || type == T_SQUOTE)
		return (TRUE);
	return (FALSE);
}

t_bool	has_a_filename_after_redirection_operators(t_token *tokens)
{
	if (!tokens)
		return (FALSE);
	while (tokens)
	{
		if (has_redirection_type(tokens))
		{
			tokens = tokens->next;
			if (!tokens)
				return (FALSE);
			skip_blank_tokens(&tokens);
			if (!token_contains_a_string(tokens))
				return (FALSE);
		}
		tokens = tokens->next;
	}
	return (TRUE);
}

t_bool	has_a_valid_command_before_and_after_pipes(t_token *tokens)
{
	t_bool	has_met_a_string_token_since_last_pipe;

	has_met_a_string_token_since_last_pipe = FALSE;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
		{
			if (!has_met_a_string_token_since_last_pipe)
				return (FALSE);
			has_met_a_string_token_since_last_pipe = FALSE;
		}
		else if (token_contains_a_string(tokens))
			has_met_a_string_token_since_last_pipe = TRUE;
		tokens = tokens->next;
	}
	return (has_met_a_string_token_since_last_pipe);
}

t_bool	line_contains_only_blanks(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t')
			return (FALSE);
		line++;
	}
	return (TRUE);
}

int syntax_checker(char *line, t_token *tokens)
{
	(void)tokens;
	if (line_contains_only_blanks(line))
		return (1);
	if (contains_unclosed_quotes(line))
	{
		printf("There are unclosed quotes in line\n");
		return (1);
	}
	if (!has_a_filename_after_redirection_operators(tokens))
	{
		printf("syntax error near redirection operator\n");
		return (1);
	}
	if (!has_a_valid_command_before_and_after_pipes(tokens))
	{
		printf("syntax error near '|'\n");
		return (1);
	}
	return (0);
}
