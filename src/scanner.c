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

char	*ft_strcdup(char *str, char c)
{
	int		length;
	char	*result;
	int		i;

	length = 1;
	while (str[length] && str[length] != c)
		length++;
	if (str[length] == c)
		length++;
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
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

char *ft_strdup_set(char *str, char *set)
{
	int		length;
	char	*result;
	int		i;

	length = 0;
	while (str[length] && !str_has(set, str[length]))
		length++;
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
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
	(*index)++;
	return (0);
}

t_token	*scanner(char *line)
{
	t_token	*current_token;
	int		len;
	char	c;
	int		i;

	i = 0;
	len = str_len(line);
	if (len == 0)
		return (NULL);
	current_token = malloc(sizeof(t_token));
	if (!current_token)
		return (NULL);
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
			return (current_token);
		else
		{
			current_token->next = malloc(sizeof(t_token));
			if (!current_token->next)
				return (NULL);
			current_token = current_token->next;
		}
	}
	return (current_token);
}

void	print_tokens(t_token *tokens)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		printf("type: %c, data: <%s>\n", current_token->type, current_token->data);
		current_token = current_token->next;
	}
}
