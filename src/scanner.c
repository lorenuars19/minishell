#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>


t_token_type	get_char_type(char c)
{
	// static char	*special_char = "|\'\" <>";

	// printf("<%s>\n", special_char);
	if (str_has(SPECIAL_CHARS, c))
		return (c);
	return (T_GENERAL);
}


int	get_quote_token(char *line, t_token *token, int *index)
{
	token->type = get_char_type(*line);
	token->data = ft_strcdup(line, token->type);
	if (token->data == NULL)
		return (MALLOC_ERROR);
	(*index) += str_len(token->data);
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

int get_space_token(char *line, t_token *token, int *index)
{
	token->type = T_SPACE;
	token->data = NULL;
	while (*line == ' ')
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

t_token	*scanner(char *line)
{
	t_token	*tokens;
	t_token	*current_token;
	int		len;
	char	c;
	int		i;

	i = 0;
	len = str_len(line);
	if (len == 0)
		return (NULL);
	tokens = ft_calloc(1, sizeof(t_token));
	if (!tokens)
		return (NULL);
	current_token = tokens;
	while (i < len)
	{
		c = line[i];
		t_token_type type = get_char_type(c);
		if (type == T_DQUOTE || type == T_SQUOTE)
			get_quote_token(line + i, current_token, &i);
		else if (type == T_SPACE)
			get_space_token(line + i, current_token, &i);
		else if (type == T_PIPE)
			get_pipe_token(line + i, current_token, &i);
		else if (type == T_GENERAL)
			get_general_token(line + i, current_token, &i);
		if (i == len)
			return (tokens);
		else
		{
			current_token->next = ft_calloc(1, sizeof(t_token));
			if (!current_token->next)
			{
				free_tokens(tokens);
				return (NULL);
			}
			current_token = current_token->next;
		}
	}
	return (tokens);
}
