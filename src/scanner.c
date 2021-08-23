#include "minishell.h"


t_token_type	get_token_type(char c)
{
	if (str_has(SPECIAL_CHARS, c))
		return (c);
	return (T_GENERAL);
}

int	get_quote_token(char *line, t_token *token, int *index)
{
	token->type = get_token_type(*line);
	token->data = ft_strcdup(line, token->type);
	if (token->data == NULL)
		return (MALLOC_ERROR);
	(*index) += str_len(token->data) + 2;
	return (0);
}

int	get_general_token(char *line, t_token *token, int *index)
{
	token->type = T_GENERAL;
	token->data = ft_strdup_set(line, SPECIAL_CHARS);
	if (token->data == NULL)
		return (MALLOC_ERROR);
	(*index) += str_len(token->data);
	return (0);
}

int get_blank_token(char *line, t_token *token, int *index)
{
	token->type = T_BLANK;
	token->data = NULL;
	while (*line == ' ' || *line == '\t')
	{
		line++;
		(*index)++;
	}
	return (0);
}

int get_pipe_token(char *line, t_token *token, int *index)
{
	(void)line;
	token->type = T_PIPE;
	token->data = NULL;
	(*index)++;
	return (0);
}

int get_redirection_token(char *line, t_token *token, int *index)
{
	if (*line == '<' && *(line + 1) == '<')
	{
		token->type = T_HEREDOC;
		*index += 2;
	}
	else if (*line == '>' && *(line + 1) == '>')
	{
		token->type = T_APPEND;
		*index += 2;
	}
	else
	{
		token->type = get_token_type(*line);
		(*index)++;
	}
	return (0);
}

int		get_next_token(char *line, t_token *token, int *i)
{
	t_token_type	type;
	int				ret;

	type = get_token_type(line[*i]);
	if (type == T_DQUOTE || type == T_SQUOTE)
		ret = get_quote_token(line + *i, token, i);
	else if (type == T_SPACE || type == T_TAB)
		ret = get_blank_token(line + *i, token, i);
	else if (type == T_PIPE)
		ret = get_pipe_token(line + *i, token, i);
	else if (type == T_GENERAL)
		ret = get_general_token(line + *i, token, i);
	else if (type == T_SMALLER || type == T_GREATER)
		ret = get_redirection_token(line + *i, token, i);
	return (ret);
}

t_token	*get_tokens(char *line, t_token *tokens, int len, int i)
{
	t_token	*current_token;

	current_token = tokens;
	while (i < len)
	{
		if (get_next_token(line, current_token, &i) != 0)
		{
			free_tokens_incl_data(tokens);
			return (NULL);
		}
		if (i == len)
			return (tokens);
		else
		{
			current_token->next = ft_calloc(1, sizeof(t_token));
			if (!current_token->next)
			{
				free_tokens_incl_data(tokens);
				return (NULL);
			}
			current_token = current_token->next;
		}
	}
	return (tokens);
}

t_token	*scanner(char *line)
{
	t_token	*tokens;
	int		len;
	int		i;

	len = str_len(line);
	if (len == 0)
		return (NULL);
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	tokens = ft_calloc(1, sizeof(t_token));
	if (!tokens)
		return (NULL);
	return (get_tokens(line, tokens, len, i));
}
