#include "minishell.h"

static t_bool	contains_unclosed_quotes(char *line)
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

static t_bool	has_a_filename_after_redirection_operators(t_token *tokens)
{
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

static t_bool	has_a_valid_command_before_and_after_pipes(t_token *tokens)
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

int	syntax_checker(char *line, t_token *tokens)
{
	if (!tokens)
		return (1);
	if (line_contains_only_blanks(line))
		return (1);
	if (contains_unclosed_quotes(line))
	{
		put_str_fd(STDERR_FILENO, "unclosed quotes in line\n");
		return (1);
	}
	if (!has_a_filename_after_redirection_operators(tokens))
	{
		put_str_fd(STDERR_FILENO, "syntax error near redirection operator\n");
		return (1);
	}
	if (!has_a_valid_command_before_and_after_pipes(tokens))
	{
		put_str_fd(STDERR_FILENO, "syntax error near '|'\n");
		return (1);
	}
	return (0);
}
